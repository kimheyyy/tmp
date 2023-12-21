#include "../Command.hpp"

int Command::pass(int fd, std::string pwd, std::list<Client> &cList)
{
	size_t i;
	for (i = 0; i < _splitMsg.size(); i++)
	{
		if (_splitMsg[i].compare("PASS") == 0)
			break;
	}
	if (i + 1 == _splitMsg.size() || _splitMsg[i + 1].compare(pwd) != 0)
	{
		std::cout << i + 1 << " " << _splitMsg[0] << " " << _splitMsg[1] << " " << pwd << "\n";
		send_fd(fd, ERR_PASSWDMISMATCH);
		return (-1);
	}
	Client nClient("", "", "", fd, S2_PASSWORD); // Client 객체 생성
	cList.push_back(nClient);					 // 목록에 추가
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