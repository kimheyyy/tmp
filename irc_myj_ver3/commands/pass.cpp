#include "../Command.hpp"

int Command::pass(Client &client, std::string pwd, std::list<Client> &cList)
{
	size_t i;
	for (i = 0; i < _splitMsg.size(); i++)
	{
		if (_splitMsg[i].compare("PASS") == 0)
			break;
	}
	if (i + 1 == _splitMsg.size() || _splitMsg[i + 1].compare(pwd) != 0)
	{
		send_fd(client.get_fd(), ERR_PASSWDMISMATCH);
		return (-1);
	}
	client.set_flag(S2_PASSWORD);
	for (i = 0; i < _splitMsg.size(); i++)
	{
		if (_splitMsg[i].compare("NICK") == 0)
		{
			std::list<Channel> chList;
			nick(cList.back(), cList, chList);
			break;
		}
	}
	return (1);
}