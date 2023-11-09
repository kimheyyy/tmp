#include "../Command.hpp"

int Command::connect(int fd, std::string pwd, std::list<Client> &cList) {	
	Client nClient("", "", "", fd, S1_CONNECT);
	cList.push_back(nClient);
	if (_splitMsg.size() != 2 && _splitMsg[2] == "PASS") {
		pass(cList.back(), pwd, cList);
	}
	return (1);
}