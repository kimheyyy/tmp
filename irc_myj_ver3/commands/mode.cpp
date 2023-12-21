#include "../Command.hpp"

bool Command::is_completed(const Client &sender)
{
    return sender.get_status() == COMPLETED;
}

int Command::modeI(const Client &client, std::list<Channel> &chList)
{
    if (_splitMsg.size() < 3 || !is_completed(client))
        return (-1);
    std::string chName = _splitMsg[1];
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nick(), chName));
        return (-1);
    }
    if (!_conChit->check_auth(client.get_nick()))
    {
        send_fd(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nick(), chName));
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (_splitMsg[2] == "+i")
    {
        channel.set_inviteOnly(true); // 초대 전용 설정 활성화
        send_fd(client.get_fd(), RPL_MODE_I(client.get_nick(), chName, _splitMsg[2]));
    }
    else if (_splitMsg[2] == "-i")
    {
        channel.set_inviteOnly(false); // 초대 전용 설정 비활성화
        send_fd(client.get_fd(), RPL_MODE_I(client.get_nick(), chName, _splitMsg[2]));
    }
    return (1);
}

int Command::modeN(const Client &sender, const std::list<Channel> &chList)
{
    if (!(_splitMsg.size() > 2) || !is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    int cFd = sender.get_fd();
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(cFd, ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
    }
    if (!_conChit->check_auth(sender.get_nick()))
    {
        send_fd(cFd, ERR_CHANOPRIVSNEEDED(sender.get_nick(), chName));
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (_splitMsg[2] == "+n")
    {
        channel.set_allowMsg(true); // 외부 메시지 허용 설정 활성화
        send_fd(cFd, RPL_MODE_PLUS_N(sender.get_nick(), chName));
    }
    else if (_splitMsg[2] == "-n")
    {
        channel.set_allowMsg(false); // 외부 메시지 허용 설정 비활성화
        send_fd(cFd, RPL_MODE_MINUS_N(sender.get_nick(), chName));
    }
    return (1);
}

int Command::modeT(const Client &sender, const std::list<Channel> &chList)
{
    if (!(_splitMsg.size() > 2) || !is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    int cFd = sender.get_fd();
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(cFd, ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
    }
    if (!_conChit->check_auth(sender.get_nick()))
    {
        send_fd(cFd, ERR_CHANOPRIVSNEEDED(sender.get_nick(), chName));
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (_splitMsg[2] == "+t")
    {
        channel.set_topicProtection(true); // 주제 보호 설정 활성화
        send_fd(cFd, RPL_MODE_T(sender.get_nick(), chName, _splitMsg[2]));
    }
    else if (_splitMsg[2] == "-t")
    {
        channel.set_topicProtection(false); // 주제 보호 설정 비활성화
        send_fd(cFd, RPL_MODE_T(sender.get_nick(), chName, _splitMsg[2]));
    }
    return (1);
}

int Command::modeK(const Client &sender, std::list<Channel> &chList)
{
    if (!(_splitMsg.size() > 2) || !is_completed(sender))
        return (-1);
    std::string chName = _splitMsg[1];
    int cFd = sender.get_fd();
    _conChit = check_validChannel(chName, chList);
    if (_conChit == chList.end())
    {
        send_fd(cFd, ERR_NOSUCHCHANNEL(sender.get_nick(), chName));
    }
    if (!_conChit->check_auth(sender.get_nick()))
        return (-1);
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (_splitMsg[2] == "+k")
    {
        channel.setPassword(_splitMsg[3]); // 비밀번호 설정
        send_fd(cFd, RPL_MODE_K(sender.get_nick(), chName, _splitMsg[3]));
    }
    else if (_splitMsg[2] == "-k")
    {
        channel.removePassword(); // 비밀번호 제거
        send_fd(cFd, RPL_MODE_K(sender.get_nick(), chName, _splitMsg[3]));
    }
    return (1);
}

int Command::modeL(const Client &sender, std::list<Channel> &chList)
{
    if (!(_splitMsg.size() > 2) || !is_completed(sender))
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
    if (_splitMsg[2] == "+l")
    {
        if (_splitMsg.size() < 4)
            return (-1);
       	int limit = atoi(_splitMsg[3].c_str());
        channel.setUserLimit(limit); // 사용자 수 제한 설정
        send_fd(cFd, RPL_MODE_L(sender.get_nick(), chName, _splitMsg[3]));
    }
    else if (_splitMsg[2] == "-l")
    {
        channel.removeUserLimit(); // 사용자 수 제한 제거
        send_fd(cFd, RPL_MODE_L(sender.get_nick(), chName, "No limit"));
    }
    return (1);
}