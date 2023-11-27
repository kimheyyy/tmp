#include "../Command.hpp"

bool Command::is_completed(const Client &sender)
{
    return sender.get_status() == COMPLETED;
}

bool Command::is_validSize()
{
    return _splitMsg.size() >= 2;
}

int Command::modeI(const Client &sender)
{
    if (!is_completed(sender))
        return (-1);
    send_fd(sender.get_fd(), RPL_MODE_I(sender.get_nick(), sender.get_ip()));
    return (1);
}

int Command::modeN(const Client &sender, const std::list<Channel> &chList)
{
    if (!is_validSize() || !is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    int cFd = sender.get_fd();
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(cFd, ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
    }
    else
    {
        send_fd(cFd, RPL_MODE_N(sender.get_nick(), chName, "3"));
    }
    return (1);
}