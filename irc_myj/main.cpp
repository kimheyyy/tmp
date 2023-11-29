#include <poll.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Server.hpp"

// 인자를 확인하는 함수
int check_arg(int argc, char *argv[])
{
    // 인자의 수가 3개가 아니면 에러 메시지를 출력하고 -1을 반환
    if (argc != 3)
    {
        std::cout << "Error : Check argument number\n";
        std::cout << "Usage: ./irserve <portnum> <password>\n";
        return (-1);
    }
    // 첫 번째 인자(포트 번호)가 숫자인지 확인
    for (int i = 0; argv[1][i] != '\0'; i++)
    {
        if (argv[1][i] < '0' || argv[1][i] > '9')
        {
            std::cout << "Error : Check port num\n";
            return (-1);
        }
    }
    // 포트 번호가 1024 이상 9999 이하인지 확인
    int portNum = std::atoi(argv[1]);
    if (portNum < 1024 || portNum > 9999)
    {
        std::cout << "Error : Check port num\n";
        return (-1);
    }
    // 포트 번호 반환
    return (portNum);
}

// 메인 함수
int main(int argc, char *argv[]) // /a.out 포트번호 비밀번호
{
    // 인자 확인
    int portNum = check_arg(argc, argv); // 인자 수 3개, 포트번호가 숫자인지, 포트번호가 1024이상 9999이하인지
    if (portNum == -1)
        return (1);

    // 서버 객체 생성
    Server server(portNum, argv[2]);
    int ret;
    int serverFd = server.get_serverFd();          // 서버 소켓의 파일 디스크립터 반환
    struct pollfd *serverPoll = server.get_poll(); // pollfd 구조체 배열 반환 (pollfd 구조체는 파일 디스크립터와 이벤트를 저장하는 구조체)

    // 서버 시작 메시지 출력
    std::cout << "     __        __   _                          _         \n";
    std::cout << "     \\ \\      / /__| | ___ ___  _ __ ___   ___| |        \n";
    std::cout << "      \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ |        \n";
    std::cout << "       \\ V  V /  __/ | (_| (_) | | | | | |  __/_|        \n";
    std::cout << "        \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___(_)        \n";
    std::cout << " __        __         _                           \n";
    std::cout << " \\ \\      / /__  _ __| | _____ _ __               \n";
    std::cout << "  \\ \\ /\\ / / _ \\| '__| |/ / _ \\ '__|              \n";
    std::cout << "   \\ V  V / (_) | |  |   <  __/ |                 \n";
    std::cout << "    \\_/\\_/ \\___/|_|  |_|\\_\\___|_|                 \n";
    std::cout << std::endl;

    // 이벤트 루프
    while (1)
    {
        ret = poll(serverPoll, 100, 500); // poll 함수를 호출하여 이벤트가 발생했는지 확인
        if (ret > 0)                      // 이벤트가 발생했을 경우
        {
            if (serverPoll[serverFd].revents & POLLIN) // 서버 소켓에 이벤트가 발생했는지 확인
            {
                if (server.accept_client() == 1) // 클라이언트가 접속했을 경우
                    ret--;                       // 이벤트 발생 횟수 감소
            }
            for (int i = serverFd + 1; i < USER_MAX && ret > 0; i++) // 모든 클라이언트에 대해서 이벤트가 발생했는지 확인
            {
                if (serverPoll[i].revents & POLLIN || serverPoll[i].revents & POLLHUP) // 데이터 수신(POLLIN) 또는 연결 종료(POLLUP) 확인
                {
                    if (server.read_client(i) == 1) // 클라이언트가 데이터를 보냈을 경우
                        ret--;
                }
            }
        }
    }
    return (0);
}
