#include "../Command.hpp"

int Command::connect(int fd, std::string pwd, std::list<Client> &cList)
{
	Client nClient("", "", "", fd, S1_CONNECT);
	cList.push_back(nClient);
	if (_splitMsg.size() != 2 && _splitMsg[2] == "PASS")
	{ // connect명령어에 바로 pass인자를 넘긴경우
		pass(cList.back(), pwd, cList);
	}
	return (1);
}