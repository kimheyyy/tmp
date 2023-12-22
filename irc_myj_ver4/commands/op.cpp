#include "../Command.hpp"
int Command::op(const Client &client, std::list<Channel> &chList)
{
    if (_splitMsg.size() < 4)
        return (-1);
    else if (!is_completed(client))
        return (-1);
    std::string chName = _splitMsg[1];
    std::string nick = _splitMsg[3];
    int cFd = client.get_fd();
    std::string msg;
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        msg = ERR_NOSUCHNICK(client.get_nick(), chName);
        send_fd(cFd, msg);
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (channel.check_auth(client.get_nick()) == false)
    {
        msg = ERR_CHANOPRIVSNEEDED(client.get_nick(), chName);
        send_fd(cFd, msg);
        return (-1);
    }
    if (channel.check_client_in_ch(nick) == false)
    {
        msg = ERR_USERNOTINCHANNEL(client.get_nick(), nick, chName);
        send_fd(cFd, msg);
        return (-1);
    }
    else
    {
        channel.op_user(nick);
        msg = RPL_OP(client.get_nick(), client.get_nick(), client.get_servername(), chName, nick);
        send_all(channel.get_fds(cFd), msg);
    }
    send_fd(cFd, msg);
    return (1);
}

int Command::deop(const Client &client, std::list<Channel> &chList)
{
    if (_splitMsg.size() < 4)
        return (-1);
    else if (!is_completed(client))
        return (-1);
    std::string chName = _splitMsg[1];
    std::string nick = _splitMsg[3];
    int cFd = client.get_fd();
    std::string msg;
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        msg = ERR_NOSUCHNICK(client.get_nick(), chName);
        send_fd(cFd, msg);
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (channel.check_auth(client.get_nick()) == false)
    {
        msg = ERR_CHANOPRIVSNEEDED(client.get_nick(), chName);
        send_fd(cFd, msg);
        return (-1);
    }
    if (channel.check_client_in_ch(nick) == false)
    {
        msg = ERR_USERNOTINCHANNEL(client.get_nick(), nick, chName);
        send_fd(cFd, msg);
        return (-1);
    }
    else
    {
        channel.deop_user(nick);
        msg = RPL_DEOP(client.get_nick(), client.get_nick(), client.get_servername(), chName, nick);
        send_all(channel.get_fds(cFd), msg);
    }
    send_fd(cFd, msg);
    return (1);
}