#include "../Command.hpp"

int Command::ping(const Client &client) {
	if (_splitMsg.size() < 2)
		return (-1);
	else if (client.get_status() != COMPLETED)
		return (-1);
	send_fd(client.get_fd(), RPL_PONG(_splitMsg[1]));
	return (1);
}