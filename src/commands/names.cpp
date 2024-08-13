#include "../../inc/command.hpp"


void Command::names(std::string buffer, Client &client)
{
	(void)client;
	if (buffer.length() == 6)
	{
		std::map<int, Client *>::iterator it = _server->getClients().begin();
		while(it != _clients.end())
		{
			std::cout << (*it->second).getName();
			it++;
		}
	}
	else
	{
		std::string channel = buffer.substr(6);
	}
}
