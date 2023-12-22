#include "../Command.hpp"

int Command::user(Client &client)
{
	if (_splitMsg.size() < 5)
	{
		send_fd(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nick(), ""));
		return (-1);
	}
	size_t i;

	if (client.get_status() == S1_CONNECT)
	{
		send_fd(client.get_fd(), ERR_PASSWDMISMATCH);
		return (-1);
	}
	else if (client.get_status() == S2_PASSWORD)
	{
		send_fd(client.get_fd(), ERR_NONICKNAMEGIVEN);
		return (-1);
	}
	else if (client.get_status() == COMPLETED)
		return (-1);
	for (i = 0; i < _splitMsg.size(); i++)
	{
		if (_splitMsg[i].compare("USER") == 0)
			break;
	}
	if (_splitMsg.size() < i + 5)
	{
		send_fd(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nick(), "USER"));
		return (-1);
	}
	client.set_realname(_splitMsg[i + 4].erase(0, 1));
	client.set_servername(_splitMsg[i + 3]);
	client.set_hostname(_splitMsg[i + 2]);
	if (client.get_status() == S3_NICK)
	{
		client.set_flag(COMPLETED);
		welcome_msg(client);
	}
	return (1);
}