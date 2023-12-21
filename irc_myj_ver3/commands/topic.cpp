#include "../Command.hpp"

int Command::topic(const Client &sender, const std::list<Channel> &chList)
{
    if (!(_splitMsg.size() > 1) || !is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    int cFd = sender.get_fd();
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(cFd, ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);

    if (_splitMsg.size() > 2) // 주제변경
    {
        if (channel.get_topicProtection())
        {
            if (!channel.check_auth(sender.get_nick()))
            {

                send_fd(cFd, ERR_CHANOPRIVSNEEDED(sender.get_nick(), chName));
                return (-1);
            }
            channel.set_topic(_splitMsg[2]); // 주제 설정
        }
        else
        {
            channel.set_topic(_splitMsg[2]); // 주제 설정
        }
        send_all(channel.get_fds(cFd), RPL_TOPIC(sender.get_nick(), chName, _splitMsg[2]));
        send_fd(cFd, RPL_TOPIC(sender.get_nick(), chName, _splitMsg[2]));
    }
    else
    {
        std::string current_topic = channel.get_topic();
        if (current_topic.empty())
        {
            send_fd(cFd, RPL_NOTOPIC(sender.get_nick(), chName));
        }
        else
        {
            send_fd(cFd, RPL_TOPIC(sender.get_nick(), chName, current_topic));
        }
    }
    return (1);
}
