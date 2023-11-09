#include "../Command.hpp"

int Command::kick(const Client &client, std::list<Channel> &chList) {
	if (_splitMsg.size() < 3)//들어온 메시지가 3개인지 검사
		return (-1);
	else if (client.get_status() != COMPLETED)//해당 클라이언트 상태검사
		return (-1);
	std::string chName = _splitMsg[1];
	std::string target = _splitMsg[2];
	_conChit = check_validChannel(chName, chList);//유효한 채널인지 확인
	if (_conChit == chList.end()) {//유효하지 못한 채널인 경우
		send_fd(client.get_fd(), ERR_NOSUCHCHANNEL(client.get_nick(), chName));
		return (-1);
	}
	Channel &channel = const_cast<Channel &>(*_conChit);
	if (!channel.check_auth(client.get_nick())) {//사용자 권한확인
		send_fd(client.get_fd(), ERR_CHANOPRIVSNEEDED(client.get_nick(), chName));
		return (-1);
	}
	if (!channel.check_client_in_ch(target)) {//채널에 클라이언트가 있는지 확인
		send_fd(client.get_fd(), ERR_USERNOTINCHANNEL(client.get_nick(), target, chName));
		return (-1);
	}
	std::string msg = "";
	if (_splitMsg.size() > 3)
		msg = _splitMsg[3];
	send_fd(client.get_fd(), RPL_KICK(client.get_nick(), client.get_nick(), client.get_ip(), chName, target, msg));
	send_all(channel.get_fds(client.get_fd()), RPL_KICK(client.get_nick(), client.get_nick(), client.get_ip(), chName, target, msg));
	channel.rm_byNick(target);//해당 클라이언트를 체널에서 제거
	if (channel.get_userSize() == 0)//채널에 더 이상 유저가 없으면
		remove_channel(chList, _conChit->get_name());
    return (1);
}