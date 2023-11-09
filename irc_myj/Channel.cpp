#include "Channel.hpp"

Channel::Channel(const Client &client, const std::string &name)
    : _name(name)
{
    _userModes.insert(std::make_pair(client.get_nick(), OPER));
    _users.insert(std::make_pair(client.get_nick(), client));
}

const std::string Channel::get_name() const
{
    return _name;
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
        _userModes.insert(std::make_pair(nick, OPER));
    }
}

void Channel::deop_user(const std::string &nick)
{
    if (check_client_in_ch(nick))
    {
        _userModes.insert(std::make_pair(nick, NORMAL));
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