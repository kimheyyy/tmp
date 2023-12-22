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
    bool _allowMsg;

    bool _topicProtection; // 주제 보호 상태를 저장하는 변수
    std::string _topic;    // 채널의 주제를 저장하는 변수

    bool _inviteOnly; // 초대 전용 설정을 저장하는 변수

    std::map<std::string, int> _inviteList; // 초대된 사용자의 닉네임을 저장하는 맵

    std::string _password;

    int _userLimit;

public:
    Channel(const Client &client, const std::string &name);
    const std::string get_name() const;
    bool check_auth(const std::string &nick) const;
    bool check_client_in_ch(const std::string &nick) const;

    void add_user(const Client &client);
    void rm_user(const std::string &nick);
    int rm_byNick(const std::string &nick);

    int get_userSize() const;
    std::string get_usersNames() const;
    std::vector<int> get_fds(int senderFd) const;

    void op_user(const std::string &nick);
    void deop_user(const std::string &nick);
    bool already_exists(const std::string &nick);

    bool get_allowMsg() const;
    void set_allowMsg(bool allow);

    bool get_topicProtection() const;
    void set_topicProtection(bool new_topicProtection);
    void set_topic(const std::string &new_topic);
    std::string get_topic() const;

    void set_inviteOnly(bool value); // 초대 전용 설정을 설정하는 메서드
    bool get_inviteOnly() const;     // 초대 전용 설정을 가져오는 메서드

    void inviteUser(const std::string &nickname);
    void removeInvite(const std::string &nickname);
    bool isInvited(const std::string &nick);

    void setPassword(const std::string &newPassword);
    void removePassword();
    bool check_password(const std::string &password);
    bool hasPassword() const;

    void setUserLimit(int limit);
    void removeUserLimit();
    int get_userLimit() const;
};

#endif // CHANNEL_H