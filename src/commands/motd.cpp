#include "../../inc/command.hpp"

void Command::motd(Client &client)
{
	std::string motd = "Welcome to the BEST IRC SERVER OMGGGG";

	send_response(client.getFd(), "375", "*", "MOTD - start", _server->getServerName());
	send_response(client.getFd(), "372", "*", motd, _server->getServerName());
	send_response(client.getFd(), "376", "*", "End of MOTD", _server->getServerName());
}