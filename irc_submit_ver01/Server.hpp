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
    struct sockaddr_in _serverSin; // 서버와 클라이언트의 ip,port num을 저장하고 있는 타입,네트워크 통일된 방식으로 주송정보를 교환할수 있도록 돕는다.소켓 API 함수가 일반적으로 struct sockaddr * 형식의 인자를 요구
    struct pollfd _poll[USER_MAX]; // 감시할 파일디스크립터와 이벤트 그리고 발생한 이벤트를 저장하는데 사용
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
    void execute_command(int fd, Command &com);

    struct pollfd *get_poll(void);
    int get_serverFd(void);
    void set_pollFd(int index, int fd, int events, int revents);
    std::list<Client>::iterator get_clientFd(int fd);
};

#endif
