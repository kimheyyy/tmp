#include "../Command.hpp"

int Command::list(const Client &client, const std::list<Channel> &chList)
{
    if (client.get_status() != COMPLETED)
        return (-1);
    int cFd = client.get_fd();
    std::string msg;
    msg = RPL_LISTSTART(client.get_nick());
    send_fd(cFd, msg);
    std::list<Channel>::const_iterator coit;
    for (coit = chList.begin(); coit != chList.end(); coit++)
    {
        std::ostringstream ss;
        ss << coit->get_userSize();
        std::string numUsersStr = ss.str();

        msg = RPL_LIST(client.get_nick(), coit->get_name(), numUsersStr);
        send_fd(cFd, msg);
    }
    msg = RPL_LISTEND(client.get_nick());
    send_fd(cFd, msg);
    return (1);
}