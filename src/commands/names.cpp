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

/*void Command::names(std::string buffer) 
{
    if (buffer.length() == 6) {
        std::map<int, Client *>::iterator it = _server->getClients().begin();
        std::string users;
        while (it != _server->getClients().end()) 
		{
            users += it->second->getNickname() + " ";
            it++;
        }
        send_response(it->second->getFd(), "353", "=", users, _server->getServerName());
    } 
	else 
	{
        std::string channel = buffer.substr(6);

        Channel *chan = _server->findChannel(channel);
        if (chan == 0) 
		{
            std::map<int, Client *>::iterator it = _server->getClients().begin();
            send_response(it->second->getFd(), "403", channel, " :No such channel", _server->getServerName());
            return;
        }

        std::string users;
        Client **clients = chan->getClients(); 
        int numClients = chan->getNumClients();
        for (int i = 0; i < numClients; i++) 
		{
            users += clients[i]->getNickname() + " ";
        }
        std::map<int, Client *>::iterator it = _server->getClients().begin();
        send_response(it->second->getFd(), "353", channel, users, _server->getServerName());
        send_response(it->second->getFd(), "366", channel, " :End of NAMES list", _server->getServerName());
    }
}*/
