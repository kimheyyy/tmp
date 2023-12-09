#include "Channel.hpp"

Channel::Channel(const Client &client, const std::string &name)
    : _name(name), _topicProtection(false), _topic(""), _inviteOnly(false), _password(""), _userLimit(-1)
{
    _userModes.insert(std::make_pair(client.get_nick(), OPER));
    _users.insert(std::make_pair(client.get_nick(), client));
    _allowMsg = true;
}

const std::string Channel::get_name() const
{
    return _name;
}

bool Channel::get_topicProtection() const
{
    return _topicProtection;
}

void Channel::set_topicProtection(bool new_topicProtection)
{
    _topicProtection = new_topicProtection;
}

void Channel::set_topic(const std::string &new_topic)
{
    _topic = new_topic;
}

std::string Channel::get_topic() const
{
    return _topic;
}

bool Channel::check_auth(const std::string &nick) const
{
    std::map<std::string, int>::const_iterator it = _userModes.find(nick);
    if (it != _userModes.end() && it->second == OPER)
    {
        return true;
    }
    return false;
}

bool Channel::check_client_in_ch(const std::string &nick) const
{
    return _users.find(nick) != _users.end();
}

void Channel::add_user(const Client &client)
{
    _userModes.insert(std::make_pair(client.get_nick(), NORMAL));
    _users.insert(std::make_pair(client.get_nick(), client));
}

void Channel::rm_user(const std::string &nick)
{
    _userModes.erase(nick);
    _users.erase(nick);
}

int Channel::rm_byNick(const std::string &nick)
{
    if (check_client_in_ch(nick))
    {
        rm_user(nick);
        return 1;
    }
    return 0;
}

int Channel::get_userSize() const
{
    return _users.size();
}

std::string Channel::get_usersNames() const
{
    std::string names;
    for (std::map<std::string, Client>::const_iterator it = _users.begin(); it != _users.end(); ++it)
    {
        names += it->first + " ";
    }
    return names;
}

std::vector<int> Channel::get_fds(int senderFd) const
{
    std::vector<int> fds;
    for (std::map<std::string, Client>::const_iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if (it->second.get_fd() != senderFd)
        {
            fds.push_back(it->second.get_fd());
        }
    }
    return fds;
}

void Channel::op_user(const std::string &nick)
{
    if (check_client_in_ch(nick))
    {
        std::map<std::string, int>::iterator it = _userModes.find(nick);
        if (it != _userModes.end())
        {
            it->second = OPER;
        }
    }
}

void Channel::deop_user(const std::string &nick)
{
    if (check_client_in_ch(nick))
    {
        std::map<std::string, int>::iterator it = _userModes.find(nick);
        if (it != _userModes.end())
        {
            it->second = NORMAL;
        }
    }
}
bool Channel::already_exists(const std::string &nick)
{
    if (check_client_in_ch(nick))
    {
        return true;
    }
    return false;
}

bool Channel::get_allowMsg() const
{
    return _allowMsg;
}

void Channel::set_allowMsg(bool allow)
{
    _allowMsg = allow;
}

void Channel::inviteUser(const std::string &nickname)
{
    _inviteList.insert(std::make_pair(nickname, true));
}

bool Channel::isInvited(const std::string &nick)
{
    // 초대 목록에서 사용자를 찾습니다.
    std::map<std::string, int>::const_iterator it = _inviteList.find(nick);
    // 사용자가 초대 목록에 있으면 true를 반환하고, 그렇지 않으면 false를 반환합니다.
    return it != _inviteList.end();
}

void Channel::removeInvite(const std::string &nickname)
{
    _inviteList.erase(nickname);
}

void Channel::set_inviteOnly(bool value)
{
    _inviteOnly = value;
}

bool Channel::get_inviteOnly() const
{
    return _inviteOnly;
}

void Channel::setPassword(const std::string &newPassword)
{
    _password = newPassword;
}

void Channel::removePassword()
{
    _password.clear();
}

bool Channel::check_password(const std::string &password) // const 괄호뒤에도 붙여야 하나?
{
    // 비밀번호가 설정되어 있지 않으면 true를 반환
    if (_password.empty())
        return true;
    // 비밀번호가 설정되어 있으면 입력된 비밀번호와 비교
    return _password == password;
}

void Channel::setUserLimit(int limit)
{
    _userLimit = limit;
}

void Channel::removeUserLimit()
{
    _userLimit = -1; // -1은 사용자 수 제한이 없음을 나타냅니다.
}

int Channel::get_userLimit() const
{
    return _userLimit;
}
