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
    int _serverFd;                      // 서버 소켓의 파일 디스크립터
    struct sockaddr_in _serverSin;      // 서버 소켓의 주소 정보
    struct pollfd _poll[USER_MAX];      // pollfd 구조체 배열
    std::string _pwd;                   // 비밀번호
    std::list<Client> _clients;         // 클라이언트 리스트
    std::list<Channel> _channels;       // 채널 리스트
    std::list<Client>::iterator _cit;   // 클라이언트 리스트의 반복자
    std::list<Channel>::iterator _chit; // 채널 리스트의 반복자
    char _readBuf[BUF];                 // read() 함수로 읽어온 데이터를 저장하는 버퍼
    char _saveBuf[USER_MAX][BUF * 2];   // 이중 버퍼를 사용하는 이유는, read()로 읽어온 데이터를 저장하는 버퍼와 strtok()로 토큰을 분리한 데이터를 저장하는 버퍼를 분리 사용하기 위함
    // strtok()이란? 문자열을 특정 문자로 구분하여 토큰으로 분리하는 함수
public:
    Server(int portNum, std::string pwd);

    std::list<Channel>::iterator check_Channel(std::string channel); // 채널이 있는지 확인하는 함수
    int accept_client(void);                                         // 클라이언트를 받아들이는 함수
    int read_client(int fd);                                         // 클라이언트로부터 데이터를 읽어오는 함수
    void execute_command(int fd);                                    // 클라이언트로부터 받아온 데이터를 처리하는 함수

    struct pollfd *get_poll(void);                               // pollfd 구조체 배열을 반환하는 함수
    int get_serverFd(void);                                      // 서버 소켓의 파일 디스크립터를 반환하는 함수
    void set_pollFd(int index, int fd, int events, int revents); // pollfd 구조체 배열의 값을 설정하는 함수
    std::list<Client>::iterator get_clientFd(int fd);            // 클라이언트 리스트의 반복자를 반환하는 함수
};

#endif
