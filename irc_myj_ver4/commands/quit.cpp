#include "../Command.hpp"
int Command::quit(std::list<Client>::iterator cIt, std::list<Channel> &chList, std::list<Client> &cList)
{
    std::vector<int> mList;
    std::vector<int> temp;

    if (!is_completed(*cIt) || cIt == cList.end())
        return (-1);
    std::string nick = cIt->get_nick();
    std::string ip = cIt->get_servername();
    int fd = cIt->get_fd();
    for (_chit = chList.begin(); _chit != chList.end();)
    {
        if (_chit->rm_byNick(nick) == 1)
            temp = _chit->get_fds(fd);
        mList.insert(mList.end(), temp.begin(), temp.end());
        if (_chit->get_userSize() == 0)
            chList.erase(_chit++);
        else
            ++_chit;
    }
    sort(mList.begin(), mList.end());
    mList.erase(unique(mList.begin(), mList.end()), mList.end());
    std::string msg = "";
    if (_splitMsg.size() > 1)
        msg = _splitMsg[1];
    send_all(mList, RPL_QUIT(nick, nick, ip, msg));
    cList.erase(cIt);
    close(fd);
    return (1);
}
