#include "../../inc/command.hpp"

void Command::quit(Client &client)
{
	std::string response = ":" + _server->getServerName() + " QUIT :Quit: See you !" + "\r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);
	close(client.getFd());

	std::map<int, Client *> &all_client = _server->getClients();
	delete all_client[client.getFd()];
	all_client.erase(client.getFd());
}
