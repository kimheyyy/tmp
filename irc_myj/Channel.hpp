// Channel.hpp 파일

#ifndef CHANNEL_H
#define CHANNEL_H

#include <map>
#include <vector>
#include <string>
#include <iterator>
#include "Client.hpp"
#include "Define.hpp"

#define OPER 1
#define NORMAL 2
#define SIZE 3

class Channel
{
private:
    std::map<std::string, int> _userModes; // 사용자 닉네임을 키로 사용하여 사용자 모드를 저장
    std::map<std::string, Client> _users;  // 사용자 닉네임을 키로 사용하여 Client 객체 저장
    std::string _name;

public:
    Channel(const Client &client, const std::string &name);
    const std::string get_name() const;
    bool check_auth(const std::string &nick) const;
    bool check_client_in_ch(const std::string &nick) const;

    void add_user(const Client &client);
    void rm_user(const std::string &nick);
    int rm_byNick(const std::string &nick);

	/* getter */
    int get_userSize() const;
    std::string get_usersNames() const;
    std::vector<int> get_fds(int senderFd) const;

    void op_user(const std::string &nick);
    void deop_user(const std::string &nick);
    bool already_exists(const std::string &nick);
};

#endif // CHANNEL_H