#include "../Command.hpp"

int Command::invite(const Client &client,std::list<Channel> &chList, const std::list<Client> &cList) {
	if (_splitMsg.size() < 3)
		return (-1);
	else if (client.get_status() != COMPLETED)
		return (-1);
	std::string target = _splitMsg[1];
	std::string chName = _splitMsg[2];
	_conChit = check_validChannel(chName, chList);
	if (_conChit == chList.end()) {
		send_fd(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nick(), chName));
		return (-1);
	}
	if (!_conChit->check_auth(client.get_nick())) {
		send_fd(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nick(), chName));
		return (-1);
	}
	_conCit = check_dupNick(_splitMsg[1], cList);
	if (_conCit == cList.end()) {
		send_fd(client.get_fd(), ERR_NOSUCHNICK(client.get_nick(), target));
		return (-1);
	}
	for (_chit = chList.begin(); _chit != chList.end(); _chit++)
	{
		if ((*_chit).get_name() == chName) {
			if(!(_chit->already_exists(_conCit->get_nick())))
				_chit->add_user((*_conCit));
			else 
				return (-1);
		}	
	}
	send_fd(client.get_fd(), RPL_INVITING(client.get_nick(), target, chName));
	send_fd(_conCit->get_fd(), RPL_INVITED(client.get_nick(), chName)); 
	return (1);
}