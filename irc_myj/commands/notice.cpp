#include "../Command.hpp"

int Command::notice(const Client &sender, const std::list<Client> &cList)
{
    if (_splitMsg.size() < 3)
        return (-1);
    else if (!is_completed(sender))
        return (-1);
    std::string cName = _splitMsg[1];
    _conCit = check_validClient(cName, cList);
    if (_conCit == cList.end())
    {
        send_fd(sender.get_fd(), ERR_NOSUCHNICK(sender.get_nick(), cName));
        return (-1);
    }
    send_fd(_conCit->get_fd(), RPL_NOTICE(sender.get_nick(), sender.get_nick(), sender.get_ip(), cName, _splitMsg[2]));
    return (1);
}

int Command::notice(const Client &sender, const std::list<Channel> &chList)
{
    if (_splitMsg.size() < 3)
        return (-1);
    else if (!is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(sender.get_fd(), ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
        return (-1);
    }
    Channel channel = *_conChit;
    if (channel.check_client_in_ch(sender.get_nick()) == false)
    {
        send_fd(sender.get_fd(), ERR_NOTONCHANNEL(sender.get_nick(), chName));
        return (-1);
    }
    std::vector<int> fds = channel.get_fds(sender.get_fd());
    send_all(fds, RPL_NOTICE(sender.get_nick(), sender.get_nick(), sender.get_ip(), chName, _splitMsg[2]));
    return (1);
}