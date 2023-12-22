#include "../Command.hpp"

int Command::pass(Client &client, std::string pwd, std::list<Client> &cList)
{
	if (_splitMsg.size() < 2)
	{
		send_fd(client.get_fd(), ERR_NEEDMOREPARAMS(client.get_nick(), ""));
		return (-1);
	}
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
	// TODO: 로그인 후에 패스워드 명령어가 들어오면?
	if (client.get_status() < S2_PASSWORD)
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