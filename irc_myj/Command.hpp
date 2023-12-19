#ifndef COMMAND_H
#define COMMAND_H

#include <list>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>

#include "Client.hpp"
#include "Channel.hpp"
#include "Define.hpp"

#define CONNECT 1
#define PASS 2
#define NICK 3
#define USER 4
#define INVITE 5
#define KICK 6
#define JOIN 7
#define PART 8
#define LIST 9
#define QUIT 10
#define PING 11
#define TOPIC 12
#define OP 13
#define DEOP 14
#define PRIVMSG 15
#define PRIVCH 16
#define NOTICE 17
#define NOTICE_CH 18
#define MODE_I 19
#define MODE_N 20
#define MODE_T 21
#define MODE_L 22
#define MODE_K 23

class Command
{
private:
    std::string _msg;
    std::vector<std::string> _splitMsg;
    std::list<Channel>::iterator _chit;
    std::list<Channel>::const_iterator _conChit;
    std::list<Client>::iterator _cit;
    std::list<Client>::const_iterator _conCit;

public:
    Command(std::string msg);
    void split_msg(void);
    int check_msgType(void);
    void send_fd(int fd, std::string str);
    void send_all(std::vector<int> fds, std::string str);
    std::list<Channel>::const_iterator check_validChannel(const std::string chName, const std::list<Channel> &chList);
    std::list<Client>::const_iterator check_validClient(const std::string nick, const std::list<Client> &cList);
    int check_validNick(const std::string nick);
    int check_nick(std::string nickName, std::list<Client> &cList, int fd);

    std::list<Client>::iterator get_clientFd(int fd, std::list<Client> cList);
    void remove_channel(std::list<Channel> &chList, std::string name);
    void welcome_msg(Client &client);

    // command list
    int connect(int fd, std::string pwd, std::list<Client> &cList);
    int pass(Client &client, std::string pwd, std::list<Client> &cList);
    int join(const Client &client, std::list<Channel> &chList);
    int part(const Client &client, std::list<Channel> &chList);
    int invite(const Client &client, std::list<Channel> &chList, const std::list<Client> &cList);
    int kick(const Client &client, std::list<Channel> &chList);
    int nick(Client &client, std::list<Client> &cList, const std::list<Channel> &chList);
    int user(Client &client);
    int list(const Client &client, const std::list<Channel> &chList);
    int quit(std::list<Client>::iterator cIt, std::list<Channel> &chList, std::list<Client> &cList);
    int ping(const Client &client);
    int topic(const Client &sender, const std::list<Channel> &chList);

    int op(const Client &client, std::list<Channel> &chList);
    int deop(const Client &client, std::list<Channel> &chList);

    int privmsg(const Client &sender, const std::list<Client> &cList);
    int privmsg(const Client &sender, const std::list<Channel> &chList);

    int notice(const Client &sender, const std::list<Client> &cList);
    int notice(const Client &sender, const std::list<Channel> &chList);

    int modeI(const Client &client, std::list<Channel> &chList);
    int modeN(const Client &sender, const std::list<Channel> &chList);
    int modeT(const Client &sender, const std::list<Channel> &chList);
    int modeK(const Client &sender, std::list<Channel> &chList);
    int modeL(const Client &sender, std::list<Channel> &chList);
    bool is_completed(const Client &sender);
};

#endif