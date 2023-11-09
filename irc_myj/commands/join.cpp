#include "../Command.hpp"

int Command::join(const Client &client, std::list<Channel> &chList) {
	if (_splitMsg.size() < 2)
		return (-1);
	else if (client.get_status() != COMPLETED)
		return (-1);
	std::string chName = _splitMsg[1];
	std::string userNames = "";
	for (_chit = chList.begin(); _chit != chList.end(); _chit++) {
		if (_chit->get_name() == chName) {
			if (_chit->get_userSize() == SIZE)
			{
				send_fd(client.get_fd(), ERR_CHANNELISFULL(client.get_nick(), chName));
				return (-1);
			}
			_chit->add_user(client);
			std::vector<int> fds = _chit->get_fds(client.get_fd());
			send_all(fds, RPL_JOIN(client.get_nick(), client.get_ip(), chName));
			userNames = _chit->get_usersNames();
			break ;
		}
	}
	if (_chit == chList.end())
	{
		Channel ch(client, chName);
		chList.push_back(ch);
		userNames = ch.get_usersNames();
	}
	send_fd(client.get_fd(), RPL_JOIN(client.get_nick(), client.get_ip(), chName));
	send_fd(client.get_fd(), RPL_NAMREPLY(client.get_nick(), chName, userNames));
	send_fd(client.get_fd(), RPL_ENDOFNAMES(client.get_nick(), chName));
	return (1);
}