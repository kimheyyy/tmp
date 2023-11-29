#ifndef SERVER_H
#define SERVER_H

#include <list>
#include <poll.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Define.hpp"

#define BUF 4096
#define USER_MAX 100

class Server
{
private:
    int _serverFd;                 // 소켓을 가지고있는 변수로(정수로서 다루어짐)
    struct sockaddr_in _serverSin; // 주소정보를 저장하고 있는 타입
    struct pollfd _poll[USER_MAX];
    std::string _pwd;
    std::list<Client> _clients;
    std::list<Channel> _channels;
    std::list<Client>::iterator _cit;
    std::list<Channel>::iterator _chit;
    char _readBuf[BUF];
    char _saveBuf[USER_MAX][BUF * 2];

public:
    Server(int portNum, std::string pwd);

    std::list<Channel>::iterator check_Channel(std::string channel);
    int accept_client(void);
    int read_client(int fd);
    void execute_command(int fd);
	
	/* getter */
    struct pollfd *get_poll(void);
    int get_serverFd(void);
	std::list<Client>::iterator get_clientFd(int fd);
	
	/*stter*/
    void set_pollFd(int index, int fd, int events, int revents);
};

#endif
