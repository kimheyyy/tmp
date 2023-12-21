#include <string>
#include "Server.hpp"

Server::Server(int portNum, std::string pwd)
{
	_pwd = pwd;
	try
	{
		if ((_serverFd = socket(PF_INET, SOCK_STREAM, 0)) == -1) // 프로토콜 패밀리(소켓이 사용할 프로토콜과 주소형식),소켓타입(tcp),프로토콜을 인자로 받아서 소켓 파일 디스크립터를 반환
			throw std::runtime_error("Socket open failed");

		_serverSin.sin_family = AF_INET;		 // 소켓이 사용할 주소체계(ipv4)와 프로토콜을 설정해서 운영체제에게 알려줌 그래야 네트워킹 가능
		_serverSin.sin_addr.s_addr = INADDR_ANY; // 모든 ip의 클라이언트를 허용
		_serverSin.sin_port = htons(portNum);	 // 포트번호를 네트워크 바이트 순서로 변환(데이터 해석의 일관성을 보장)하기위해 htons함수를 사용함

		socklen_t _serverSinLen = sizeof(_serverSin);

		if (bind(_serverFd, (struct sockaddr *)&_serverSin, _serverSinLen) == -1) // 소켓에 주소와 포트번호를 바인딩함
			throw std::runtime_error("Bind failed");

		if (listen(_serverFd, 42) == -1) // 운영체제에게 해당 소캣으로 클라이언트의 요청을 받을준비가 되었다고 알림 -> 서보소켓의 연결요청 대기 큐가 생성됨
			throw std::runtime_error("Listen failed");

		set_pollFd(_serverFd, _serverFd, POLLIN, 0); // pollfd 구조체를 설정하는 작업,1번째 인자는 소켓을 식별하는 인덱스값, 두번째는 구초체의 fd값,3번째는 구조체의 event값으로 설정
		for (int i = _serverFd + 1; i < USER_MAX; i++)
			set_pollFd(i, -1, 0, 0);				  // 사용되지 않는 구조체는 무효한 파일디스크립터(-1)로처리
		std::memset(_saveBuf, 0, BUF * 2 * USER_MAX); // 각 사용자에게 BUF * 2의 크기의 버퍼 0 값으로 초기화
	}
	catch (const std::runtime_error &e)
	{
		// 예외(소캣생성 실패가 아닌)가 발생했을 경우에 리소스 정리
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
	int clientFd = accept(_serverFd, (struct sockaddr *)&sclient, &sclient_len); // 서버소켓 큐에 있는 클라이언트의 연결을 수락 -> 클라이언트 소켓(fd)생성 -> 소켓간의 tcp 연결왼료
	if (clientFd == -1)
	{
		std::cout << "Error: socket connect fail\n";
		return (-1);
	}
	set_pollFd(clientFd, clientFd, POLLIN | POLLHUP, 0); // 3번째 인자는 해당 파일디스크립터에서 발생 할 수 있는 이벤트
	_poll[_serverFd].revents = 0;						 // 서버소켓에서 수행한 이벤트는 다시 초기화
	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1)
	{
		// fcntl 호출이 실패했을 경우의 처리
		std::cerr << "Error: Failed to set file descriptor to non-blocking mode\n";
		return -1; // 오류 코드 반환
	}
	// 해당 파일디스크립터가 블로킹 되지않도록 설정 -> 데이터를 읽거나 쓸떄 특정 클라이언트 입력이 완료되지 않아도 서버가 다른 작업을 수행가능(여러 클라이언트의 입력에 대한 병렬처리 가능)
	return 1;
}

int Server::read_client(int fd) // TODO: recv가 한번 밖에 반응 안하네?
{
	std::memset(_readBuf, 0, BUF);				   // readbuf초기화
	int r = recv(fd, _readBuf, BUF, MSG_DONTWAIT); // 논블로킹으로 입력으로 들어온 fd(소켓)의 데이터를 버퍼로 읽어옴
	if (r < 0)									   // 읽기실패
	{
		std::cout << "Error: read fail\n";
		close(fd);
		set_pollFd(fd, -1, 0, 0);
		return (-1);
	}
	else if (r == 0) // 클라이언트가 연결을 종료한 경우
	{
		std::memset(_saveBuf[fd], 0, BUF * 2);
		std::strcat(_saveBuf[fd], "QUIT :disconnected\r\n"); // 2번째 인자를 버퍼에 저장
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << _saveBuf[fd] << "\n\n";
		Command com(_saveBuf[fd]); // QUIT 명령에 대해 별도의 Command 객체 생성
		execute_command(fd, com);  // execute_command 함수에 Command 객체를 인자로 전달
		set_pollFd(fd, -1, 0, 0);
		std::memset(_saveBuf[fd], 0, BUF * 2);
		return (1);
	}

	// 제대로 읽은 경우
	std::strcat(_saveBuf[fd], _readBuf);
	std::string saveBufStr(_saveBuf[fd]);
	std::string::size_type pos;

	while ((pos = saveBufStr.find("\r\n")) != std::string::npos)
	{
		std::string command = saveBufStr.substr(0, pos);
		std::memset(_readBuf, 0, BUF);
		std::copy(command.begin(), command.end(), _readBuf);
		std::cout << "========== recv client " << fd << " ==========\n";
		std::cout << command << "\n\n";
		Command com(command);
		execute_command(fd, com);
		saveBufStr.erase(0, pos + 2);
	}
	std::strcpy(_saveBuf[fd], saveBufStr.c_str());
	return (1);
}

void Server::execute_command(int fd, Command &com)
{									// 클라이언트 소켓에서 읽어온 데이터를 command객체의 splitmsg변수에 저장해 놓고 타입검사를 한후
	int type = com.check_msgType(); // 그 결과를 받아서 대응되는 명령어함수 호출
	_cit = get_clientFd(fd);		// fd에 대응되는 클라이언트 객체를 받아온뒤
	switch (type)
	{
	/*case CONNECT:
		com.connect(fd, _pwd, _clients);
		break;*/
	case PASS:
		com.pass(fd, _pwd, _clients);
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
		com.join(fd, (*_cit), _channels);
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
	case TOPIC:
		com.topic((*_cit), _channels);
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
		com.modeI((*_cit), _channels);
		break;
	case MODE_N:
		com.modeN((*_cit), _channels);
		break;
	case MODE_T:
		com.modeT((*_cit), _channels);
		break;
	case MODE_L:
		com.modeL((*_cit), _channels);
		break;
	case MODE_K:
		com.modeK((*_cit), _channels);
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