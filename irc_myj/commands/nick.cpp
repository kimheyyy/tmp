#include "../Command.hpp"

int Command::nick(Client &client, std::list<Client> &cList, const std::list<Channel> &chList)
{
	std::vector<int> mList;
	std::vector<int> temp;
	std::string msg;
	std::string nickName;
	int cFd = client.get_fd();
	size_t i;

	if (client.get_status() < S2_PASSWORD) // 이전 단계 통과여부 확인
	{
		send_fd(cFd, ERR_PASSWDMISMATCH);
		return (-1);
	}
	for (i = 0; i < _splitMsg.size(); i++)
	{
		if (_splitMsg[i].compare("NICK") == 0)
		{								 // 해당 명령어 발견하면
			nickName = _splitMsg[i + 1]; // 그 다음 메시지 챙기고
			break;
		}
	}
	if (client.get_status() == DUPDUP && client.get_user().compare("") != 0)
	{
		if (check_nick(nickName, cList, cFd) == -1)
			return (-1);
		client.set_nick(nickName);
		welcome_msg(client);
		return (1);
	}
	if (client.get_status() == S2_PASSWORD)
	{
		if (check_nick(nickName, cList, cFd) == -1)
		{
			if (check_validClient(nickName, cList) != cList.end())
			{
				client.set_flag(DUPDUP);
				if (_splitMsg.size() > i + 2 && _splitMsg[i + 2].compare("USER") == 0)
					user(client);
				return (-1);
			}
			else
			{
				cList.erase(get_clientFd(cFd, cList));
				return (-1);
			}
		}
		client.set_nick(nickName);
		client.set_flag(S3_NICK);
		if (_splitMsg.size() > i + 2 && _splitMsg[i + 2].compare("USER") == 0)
			user(client);
	}
	else if (client.get_status() == COMPLETED) // 닉네임 바꾸는 로직
	{
		if (_splitMsg.size() < 2)
			return (-1);
		nickName = _splitMsg[1];
		if (check_nick(nickName, cList, cFd) == -1)
			return (-1);
		for (_conChit = chList.begin(); _conChit != chList.end(); _conChit++)
		{
			temp = _conChit->get_fds(client.get_fd()); // 인자로 들어온 fd를 제외한 fd를 가져옴
			mList.insert(mList.end(), temp.begin(), temp.end());
		}
		sort(mList.begin(), mList.end());
		mList.erase(unique(mList.begin(), mList.end()), mList.end()); // 중복요소 제거
		msg = RPL_NICK(client.get_nick(), client.get_nick(), client.get_ip(), nickName);
		send_fd(cFd, msg);
		send_all(mList, msg);	   // 벡터에 있는 클라이언트에게 모두 메시지 전송
		client.set_nick(nickName); // 닉네임 변경
	}
	return (1);
}