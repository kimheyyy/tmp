#include "Client.hpp"

Client::Client(std::string nick, std::string user, std::string ip, int fd, int flag)
: _nickname(nick), _username(user), _ip(ip), _clientFd(fd), _status(flag)
{}

void Client::set_nick(std::string nick) {
	_nickname = nick;
}

const std::string Client::get_nick() const {
	return (_nickname);
}

const std::string Client::get_user() const {
	return (_username);
}

const std::string Client::get_ip() const {
	return (_ip);
}

int Client::get_fd() const {
	return (_clientFd);
}

int Client::get_status() const {
	return (_status);
}

void Client::set_flag(int flag) {
	_status = flag;
}

void Client::set_ip(std::string ip) {
	_ip = ip;
}

void Client::set_user(std::string user) {
	_username = user;
}

bool Client::operator<(const Client& c) const {
	return (this->get_nick() < c.get_nick());
}
