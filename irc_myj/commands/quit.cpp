#include "../Command.hpp"
int Command::quit(std::list<Client>::iterator cIt, std::list<Channel> &chList, std::list<Client> &cList)
{
    std::vector<int> mList;
    std::vector<int> temp;

    if (!is_completed(*cIt) || cIt == cList.end())
        return (-1);
    for (_chit = chList.begin(); _chit != chList.end(); _chit++)
    {
        if (_chit->rm_byNick(cIt->get_nick()) == 1)
            temp = _chit->get_fds(cIt->get_fd());
        mList.insert(mList.end(), temp.begin(), temp.end());
        if (_chit->get_userSize() == 0)
            _chit = chList.erase(_chit);
    }
    sort(mList.begin(), mList.end());
    mList.erase(unique(mList.begin(), mList.end()), mList.end());
    std::string msg = "";
    if (_splitMsg.size() > 1)
        msg = _splitMsg[1];
    send_all(mList, RPL_QUIT(cIt->get_nick(), cIt->get_nick(), cIt->get_ip(), msg));
    cList.erase(cIt);
    close(cIt->get_fd());
    return (1);
}
