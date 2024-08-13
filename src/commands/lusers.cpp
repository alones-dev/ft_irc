#include "../../inc/command.hpp"

void Command::lusers(Client *client)
{
	int totalClients = _server->getClients().size();
	int totalChannels = _server->getChannels().size();
	int totalOperators = 0;
	int totalUnknown = 0;

	std::map<int, Client *>::iterator it = _server->getClients().begin();
	while (it != _server->getClients().end())
	{
		if ((*it->second).getIsOperator())
			totalOperators++;
		if ((*it->second).getIsUnknown())
			totalUnknown++;
		it++;
	}

	send_response(client->getFd(), "251", "*", "There are " + itos(totalClients) + " users and " + itos(totalOperators) + " operators on this server", _server->getServerName());
    send_response(client->getFd(), "252", "*", itos(totalOperators) + " :operator(s) online", _server->getServerName());
    send_response(client->getFd(), "253", "*", itos(totalUnknown) + " :invisible user(s)", _server->getServerName());
    send_response(client->getFd(), "254", "*", itos(totalChannels) + " :channels formed", _server->getServerName());
    send_response(client->getFd(), "255", "*", "I have " + itos(totalClients) + " clients and " + itos(totalChannels) + " channels", _server->getServerName());
}