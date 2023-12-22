#include "Client.hpp"

Client::Client(std::string nick, std::string user, std::string ip, std::string hostname, int fd, int flag)
	: _nickname(nick), _hostname(hostname), _servername(ip), _realname(user), _clientFd(fd), _status(flag)
{
}

void Client::set_nick(std::string nick)
{
	_nickname = nick;
}

const std::string Client::get_nick() const
{
	return (_nickname);
}

const std::string Client::get_hostname() const
{
	return (_hostname);
}

const std::string Client::get_realname() const
{
	return (_realname);
}

const std::string Client::get_servername() const
{
	return (_servername);
}

int Client::get_fd() const
{
	return (_clientFd);
}

int Client::get_status() const
{
	return (_status);
}

void Client::set_flag(int flag)
{
	_status = flag;
}
void Client::set_hostname(std::string hostname)
{
	_hostname = hostname;
}


void Client::set_servername(std::string ip)
{
	_servername = ip;
}

void Client::set_realname(std::string user)
{
	_realname = user;
}

bool Client::operator<(const Client &c) const
{
	return (this->get_nick() < c.get_nick());
}
