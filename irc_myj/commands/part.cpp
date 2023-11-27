#include "../Command.hpp"

int Command::part(const Client &client, std::list<Channel> &chList)
{
    if (_splitMsg.size() < 2) // 들어온 메시지가 2개인지 검사
        return (-1);
    else if (client.get_status() != COMPLETED) // 해당 클라이언트 상태 검사
        return (-1);
    std::string chName = _splitMsg[1];
    _conChit = check_validChannel(chName, chList); // 유효한 채널인지 확인
    if (_conChit == chList.end())
    { // 유효하지 못한 채널인 경우
        send_fd(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nick(), chName));
        return (-1);
    }
    Channel &channel = const_cast<Channel &>(*_conChit);
    if (!channel.check_client_in_ch(client.get_nick()))
    { // 채널에 클라이언트가 있는지 확인
        send_fd(client.get_fd(), ERR_USERNOTINCHANNEL(client.get_nick(), client.get_nick(), chName));
        return (-1);
    }
    send_fd(client.get_fd(), RPL_PART(client.get_nick(), client.get_ip(), chName));
    send_all(channel.get_fds(client.get_fd()), RPL_PART(client.get_nick(), client.get_ip(), chName));
    channel.rm_user(client.get_nick()); // 해당 클라이언트를 채널에서 제거
    return (1);
}