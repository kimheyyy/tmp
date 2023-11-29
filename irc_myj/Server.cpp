#include <string>
#include "Server.hpp"

Server::Server(int portNum, std::string pwd)
{
	_pwd = pwd;
	try
	{
		if ((_serverFd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
			throw std::runtime_error("Socket open failed");

		_serverSin.sin_family = AF_INET;
		_serverSin.sin_addr.s_addr = INADDR_ANY;
		_serverSin.sin_port = htons(portNum); // 포트번호로서 바이트오더?로 변환하기위해 htons함수를 사용함

		socklen_t _serverSinLen = sizeof(_serverSin);

		if (bind(_serverFd, (struct sockaddr *)&_serverSin, _serverSinLen) == -1) // 소켓에 주소와 포트번호를 바인딩함
			throw std::runtime_error("Bind failed");

		if (listen(_serverFd, 42) == -1) // 운영체제에게 해당 소캣으로 클라이언트의 요청을 받을준비가 되었다고 알림
			throw std::runtime_error("Listen failed");

		set_pollFd(_serverFd, _serverFd, POLLIN, 0); // pollfd 구조체를 설정하는 작업 1번째인자는 인덱스로서 두번째는 구초체의 fd값으,3번째는 구조체의 event값으로 설정
		for (int i = _serverFd + 1; i < USER_MAX; i++)
			set_pollFd(i, -1, 0, 0);				  // 사용되지 않는 구조체는 무효한 파일디스크립터(-1)로처리
		std::memset(_saveBuf, 0, BUF * 2 * USER_MAX); // 버퍼를 0으로 초기화
	}
	catch (const std::runtime_error &e)
	{
		// 예외가 발생했을 경우에 리소스 정리
		if (_serverFd != -1)
		{
			close(_serverFd);
		}
		std::cerr << "Server initialization failed: " << e.what() << std::endl;
		// throw;  // 예외를 다시 던져서 호출자에게 알림
	}
}

std::list<Channel>::iterator Server::check_Channel(std::string chName)
{
	for (_chit = _channels.begin(); _chit != _channels.end(); _chit++)
	{
		if (_chit->get_name() == chName)
			break;
	}
	return (_chit);
}

int Server::accept_client(void)
{
	struct sockaddr_in sclient; // 주수정보를 담고있는 타입선언
	socklen_t sclient_len = sizeof(sclient);
	int clientFd = accept(_serverFd, (struct sockaddr *)&sclient, &sclient_len); // 서버소켓 큐에 있는 클라이언트의 연결을 수락하고 새로운 파일디스크립터(상담을 위한) 반환
	if (clientFd == -1)
	{ //_serverFd는 클라이언트와의 연결(접수)을 위한 파일디스크립터
		std::cout << "Error: socket connect fail\n";
		return (-1);
	}
	set_pollFd(clientFd, clientFd, POLLIN | POLLHUP, 0); // 3번째 인자는 해당 파일디스크립터에서 발생 할 수 있는 이벤트
	_poll[_serverFd].revents = 0;						 // 수행한 이벤트는 제거
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)//입력으로 들어온 소켓(디스크립터) 블로킹(대기상태) 되지 않도록 처리
		;										   //해당 소켓의 read,write작업시 스레드나 프로세스는 다른 작업을 계속 진행가능
	return (1);
}

int Server::read_client(int fd)
{
	std::memset(_readBuf, 0, BUF);//버퍼 초기화
	int r = recv(fd, _readBuf, BUF, MSG_DONTWAIT); // 논블로킹MSG_DONTWAIT 으로 입력으로 들어온 fd(소켓)의 데이터를 버퍼로 읽어옴
	if (r < 0)//소켓에서 데이터를 읽어오는데 오류가 있을시
	{
		std::cout << "Error: read fail\n";
		close(fd);
		set_pollFd(fd, -1, 0, 0);
		return (-1);
	}
	else if (r == 0)//클라이언트가 연결을 종료한 경우
	{
		std::memset(_saveBuf[fd], 0, BUF * 2);
		std::strcat(_saveBuf[fd], "QUIT :disconnected\r\n");
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << _saveBuf[fd] << "\n\n";
		execute_command(fd);
		set_pollFd(fd, -1, 0, 0);//해당 소켓의 파일디스크립터를 무효화,감시 그리고 발생이벤트가 없는것으로 설정해둠
		std::memset(_saveBuf[fd], 0, BUF * 2);
		return (1);
	} // 데이터의 끝에 해당 문자들이 r,n이 있는지(메시지 수신이 완료) 확인
	if (_readBuf[r - 2] == '\r' && _readBuf[r - 1] == '\n')
	{
		std::strcat(_saveBuf[fd], _readBuf);
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << _saveBuf[fd] << "\n\n";
		execute_command(fd);
		_poll[fd].revents = 0;
		std::memset(_saveBuf[fd], 0, BUF * 2);
	} // 메시지 수신이 완료되지 않은경우
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