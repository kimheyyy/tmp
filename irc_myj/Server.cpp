#include <string>
#include "Server.hpp"

Server::Server(int portNum, std::string pwd)
{
	_pwd = pwd;
	try
	{
		if ((_serverFd = socket(PF_INET, SOCK_STREAM, 0)) == -1) // 소켓을 생성함 PF_INET은 IPv4 인터넷 프로토콜을 사용하겠다는 의미. SOCK_STREAM은 TCP를 사용하겠다는 의미
			throw std::runtime_error("Socket open failed");

		_serverSin.sin_family = AF_INET;		 // 주소체계를 IPv4로 설정. AF_INET은 IPv4 인터넷 프로토콜을 사용하겠다는 의미
		_serverSin.sin_addr.s_addr = INADDR_ANY; // 서버의 IP주소를 자동으로 찾아서 설정. INADDR_ANY는 서버의 IP주소를 자동으로 찾아서 설정
		_serverSin.sin_port = htons(portNum);	 // 포트번호를 설정. 포트번호는 2바이트이므로 htons()를 사용해서 리틀엔디안을 빅엔디안으로 바꿔줌 0x1234(2byte) -> 0x3412(2byte)

		socklen_t _serverSinLen = sizeof(_serverSin); // 주소의 길이를 저장하는 변수 선언 socklen_t는 부호없는 정수형

		if (bind(_serverFd, (struct sockaddr *)&_serverSin, _serverSinLen) == -1) // bind()는 소켓에 주소, 포트번호를 할당하는 함수
			throw std::runtime_error("Bind failed");

		if (listen(_serverFd, 42) == -1) // listen()은 소켓을 서버 소켓으로 설정하고 클라이언트의 접속을 기다리는 함수. 2번째 인자는 클라이언트의 최대 접속 수
			throw std::runtime_error("Listen failed");

		set_pollFd(_serverFd, _serverFd, POLLIN, 0);   // pollfd 구조체 배열의 값을 설정하는 함수. 3번째 인자는 해당 파일디스크립터에서 발생 할 수 있는 이벤트. POLLIN은 읽기 가능
		for (int i = _serverFd + 1; i < USER_MAX; i++) // pollfd 구조체 배열의 나머지 값들을 초기화
			set_pollFd(i, -1, 0, 0);				   // -1은 사용하지 않는 파일디스크립터를 의미
		std::memset(_saveBuf, 0, BUF * 2 * USER_MAX);  // 버퍼를 0으로 초기화
	}
	catch (const std::runtime_error &e)
	{
		if (_serverFd != -1) // 예외가 발생했을 경우에 리소스 정리
			close(_serverFd);
		std::cerr << "Server initialization failed: " << e.what() << std::endl;
		// throw;  // 예외를 다시 던져서 호출자에게 알림 (main 함수에서 예외처리)
	}
}

std::list<Channel>::iterator Server::check_Channel(std::string chName)
{
	for (_chit = _channels.begin(); _chit != _channels.end(); _chit++)
		if (_chit->get_name() == chName)
			break;
	return (_chit);
}

int Server::accept_client(void)
{
	struct sockaddr_in sclient; // 클라이언트의 주소를 저장하는 구조체
	socklen_t sclient_len = sizeof(sclient);
	int clientFd = accept(_serverFd, (struct sockaddr *)&sclient, &sclient_len); // 클라이언트의 접속을 기다리는 함수. 2번째 인자는 클라이언트의 주소를 저장하는 구조체
	if (clientFd == -1)															 // 클라이언트의 접속을 기다리다가 에러가 발생했을 경우
	{
		std::cout << "Error: accept fail\n";
		return (-1);
	}
	set_pollFd(clientFd, clientFd, POLLIN | POLLHUP, 0); // 3번째 인자는 해당 파일디스크립터에서 발생 할 수 있는 이벤트
	_poll[_serverFd].revents = 0;						 // 수행한 이벤트는 제거
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
		; // 해당 파일디스크립터가 블로킹 되지않도록 설정
	return (1);
}

int Server::read_client(int fd)
{
	std::memset(_readBuf, 0, BUF);
	int r = recv(fd, _readBuf, BUF, MSG_DONTWAIT); // 논블로킹으로 입력으로 들어온 fd(소켓)의 데이터를 버퍼로 읽어옴
	if (r < 0)
	{
		std::cout << "Error: read fail\n";
		close(fd);
		set_pollFd(fd, -1, 0, 0);
		return (-1);
	}
	else if (r == 0)
	{
		std::memset(_saveBuf[fd], 0, BUF * 2);
		std::strcat(_saveBuf[fd], "QUIT :disconnected\r\n");
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << _saveBuf[fd] << "\n\n";
		execute_command(fd);
		set_pollFd(fd, -1, 0, 0);
		std::memset(_saveBuf[fd], 0, BUF * 2);
		return (1);
	}														// 제대로 읽은 경우
	if (_readBuf[r - 2] == '\r' && _readBuf[r - 1] == '\n') // Carriage Return을 나타냅니다. 이것은 컴퓨터에서 줄의 시작으로 커서를 이동시키는 명령
	{
		std::strcat(_saveBuf[fd], _readBuf);
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << _saveBuf[fd] << "\n\n";
		execute_command(fd);
		_poll[fd].revents = 0;
		std::memset(_saveBuf[fd], 0, BUF * 2);
	} // 제대로 읽지 않은경우는 일단 저장해놓고
	else
		std::strcat(_saveBuf[fd], _readBuf);
	return (1);
}

void Server::execute_command(int fd)
{
	Command com(_saveBuf[fd]);		// 클라이언트 소켓에서 읽어온 데이터를 command객체의 splitmsg변수에 저장해 놓고 타입검사를 한후
	int type = com.check_msgType(); // 그 결과를 받아서 대응되는 명령어함수 호출
	_cit = get_clientFd(fd);		// fd에 대응되는 클라이언트 객체를 받아온뒤
	switch (type)
	{
	case CONNECT:
		com.connect(fd, _pwd, _clients);
		break;
	case PASS:
		com.pass((*_cit), _pwd, _clients);
		break;
	case NICK:
		com.nick((*_cit), _clients, _channels);
		break;
	case USER:
		com.user((*_cit));
		break;
	case INVITE:
		com.invite((*_cit), _channels, _clients);
		break;
	case JOIN:
		com.join((*_cit), _channels);
		break;
	case PART:
		com.part((*_cit), _channels);
		break;
	case KICK:
		com.kick((*_cit), _channels);
		break;
	case LIST:
		com.list((*_cit), _channels);
		break;
	case QUIT:
		com.quit(_cit, _channels, _clients);
		break;
	case PING:
		com.ping((*_cit));
		break;
	case OP:
		com.op((*_cit), _channels);
		break;
	case DEOP:
		com.deop((*_cit), _channels);
		break;
	case PRIVMSG:
		com.privmsg((*_cit), _clients);
		break;
	case PRIVCH:
		com.privmsg((*_cit), _channels);
		break;
	case NOTICE:
		com.notice((*_cit), _clients);
		break;
	case NOTICE_CH:
		com.notice((*_cit), _channels);
		break;
	case MODE_I:
		com.modeI((*_cit));
		break;
	case MODE_N:
		com.modeN((*_cit), _channels);
		break;
	default:
		break;
	}
}

struct pollfd *Server::get_poll(void)
{
	return _poll;
}

int Server::get_serverFd(void)
{
	return _serverFd;
}

void Server::set_pollFd(int index, int fd, int events, int revents)
{
	_poll[index].fd = fd;
	_poll[index].events = events;
	_poll[index].revents = revents;
}

std::list<Client>::iterator Server::get_clientFd(int fd)
{
	for (_cit = _clients.begin(); _cit != _clients.end(); _cit++)
	{
		if (_cit->get_fd() == fd)
			break;
	}
	return (_cit);
}
