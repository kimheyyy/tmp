#include <list>
#include <iostream>
#include <algorithm>
#include "Command.hpp"

Command::Command(std::string msg)
{
	_msg = msg;
	split_msg();
}

void Command::split_msg(void)
{
	std::string::size_type pos = _msg.find(" ", 0);
	std::string::size_type newpos = _msg.find("\r\n", 0);
	std::string::size_type colon = _msg.find(":", 0);
	std::string::size_type i = 0; // 'int' 대신 'std::string::size_type' 사용

	while (pos != std::string::npos || newpos != std::string::npos)
	{
		if (newpos != std::string::npos && (pos == std::string::npos || pos > newpos))
		{
			_splitMsg.push_back(_msg.substr(i, newpos - i));
			i = newpos + 2;
		}
		else if (pos != std::string::npos)
		{
			_splitMsg.push_back(_msg.substr(i, pos - i));
			i = pos + 1;
		}
		pos = _msg.find(" ", i);
		newpos = _msg.find("\r\n", i);
	}

	if (colon != std::string::npos && colon > i)
	{
		_splitMsg.push_back(_msg.substr(colon + 1));
	}
	else if (i < _msg.length())
	{
		_splitMsg.push_back(_msg.substr(i));
	}
}

int Command::check_msgType(void)
{
	std::string typeList[] = {"CONNECT", "PASS", "NICK", "USER", "INVITE", "KICK", "JOIN", "PART", "LIST", "QUIT", "PING", "TOPIC", "MODE", "PRIVMSG", "NOTICE"};

	for (size_t i = 0; i < sizeof(typeList) / sizeof(std::string); i++)
	{
		if (_splitMsg.size() > 0 && _splitMsg[0].find(typeList[i]) != std::string::npos)
		{
			switch (i)
			{
			case 12:
				if (_splitMsg.size() > 2 && _splitMsg[2].find("+o", 0) != std::string::npos)
					return (OP);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("-o", 0) != std::string::npos)
					return (DEOP);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("+i", 0) != std::string::npos)
					return (MODE_I);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("-i", 0) != std::string::npos)
					return (MODE_I);
				else if (_splitMsg.size() > 2 && (_splitMsg[2].find("+n", 0) != std::string::npos || _splitMsg[2].find("-n", 0) != std::string::npos))
					return (MODE_N);
				else if (_splitMsg.size() > 2 && (_splitMsg[2].find("+t", 0) != std::string::npos || _splitMsg[2].find("-t", 0) != std::string::npos))
					return (MODE_T);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("+l", 0) != std::string::npos)
					return (MODE_L);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("-l", 0) != std::string::npos)
					return (MODE_L);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("+k", 0) != std::string::npos)
					return (MODE_K);
				else if (_splitMsg.size() > 2 && _splitMsg[2].find("-k", 0) != std::string::npos)
					return (MODE_K);
				break;
			case 13:
				if (_splitMsg.size() > 1)
				{
					if (_splitMsg[1].find("#", 0) == std::string::npos)
						return (PRIVMSG);
					else
						return (PRIVCH);
				}
				break;
			case 14:
				if (_splitMsg.size() > 1)
				{
					if (_splitMsg[1].find("#", 0) == std::string::npos)
						return (NOTICE);
					else
						return (NOTICE_CH);
				}
				break;
			default:
				return (i + 1);
			}
		}
	}
	return (0);
}

void Command::send_fd(int fd, std::string str)
{
	int ret = send(fd, str.c_str(), str.size(), 0);
	if (ret == -1)
	{
		std::cerr << str.c_str() << "\n";
	}
	std::cout << "========== send client " << fd << " ==========\n";
	std::cout << str << "\n\n";
	return;
}

void Command::send_all(std::vector<int> fds, std::string str)
{
	for (size_t i = 0; i < fds.size(); i++)
		send_fd(fds[i], str);
	return;
}

std::list<Channel>::const_iterator Command::check_validChannel(const std::string chName, const std::list<Channel> &chList)
{
	for (_conChit = chList.begin(); _conChit != chList.end(); _conChit++)
	{
		if (_conChit->get_name() == chName)
			break;
	}
	return (_conChit);
}

std::list<Client>::const_iterator Command::check_validClient(const std::string nick, const std::list<Client> &cList)
{
	for (_conCit = cList.begin(); _conCit != cList.end(); _conCit++)
	{
		if (_conCit->get_nick() == nick)
			break;
	}
	return (_conCit);
}

int Command::check_validNick(const std::string nick)
{
	std::string cmdList[] = {"CONNECT", "PASS", "JOIN", "PART", "INVITE", "KICK", "NICK", "USER", "LIST", "QUIT", "PING", "MODE", "PRIVMSG", "NOTICE"};
	for (size_t i = 0; i < sizeof(cmdList) / sizeof(std::string); i++)
	{
		if (nick.compare(cmdList[i]) == 0)
			return (-1);
	}
	for (size_t i = 0; i < nick.size(); i++)
	{
		if ((!std::isalnum(nick[i]) && nick[i] != '-' && nick[i] != '_'))
			return (-1);
	}
	return (1);
}

int Command::check_nick(std::string nickName, std::list<Client> &cList, int cFd)
{
	if (nickName.empty() == true) // 공백인지 검사
	{
		send_fd(cFd, ERR_NONICKNAMEGIVEN);
		return (-1);
	}
	if (check_validClient(nickName, cList) != cList.end()) // 중복검사
	{
		send_fd(cFd, ERR_NICKNAMEINUSE(nickName));
		return (-1);
	}
	if (check_validNick(nickName) == -1) // 유효한 형식인지 검사(명령어를 사용할 수 없음)
	{
		send_fd(cFd, ERR_ERRONEUSNICKNAME(nickName));
		return (-1);
	}
	return (1);
}

std::list<Client>::iterator Command::get_clientFd(int fd, std::list<Client> cList)
{
	for (_cit = cList.begin(); _cit != cList.end(); _cit++)
	{
		if (_cit->get_fd() == fd)
			break;
	}
	return (_cit);
}

void Command::remove_channel(std::list<Channel> &chList, std::string chName)
{
	if (chList.size() == 1)
	{
		chList.clear();
		return;
	}
	for (_chit = chList.begin(); _chit != chList.end(); _chit++)
	{
		if ((*_chit).get_name() == chName)
			_chit = chList.erase(_chit);
	}
	return;
}

void Command::welcome_msg(Client &client)
{
	send_fd(client.get_fd(), RPL_WELCOME(client.get_nick()));
	send_fd(client.get_fd(), RPL_YOURHOST(client.get_nick()));
	client.set_flag(COMPLETED);
	return;
}