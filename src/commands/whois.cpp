#include "../../inc/command.hpp"

void	printMsg(Client *targetClient, Client &client, Server &server)
{
	std::string response;

	response = ":" + server.getServerName() + " 311 " + client.getNickname() + " " +
				targetClient->getNickname() + " " + targetClient->getName() 
				+ " localhost * : " + client.getRealName() + " \r\n";
	send(client.getFd(), response.c_str(), response.size(), 0);

	std::vector<std::string> channels = targetClient->getChannelsIsIn(server.getChannels(), targetClient->getFd());
	if (!channels.empty()) {
		response = ":" + server.getServerName() + " 319 " + client.getNickname() + " " +
					targetClient->getNickname() + " :" + joinVector(channels, " ") + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	}
}


void Command::whois(std::string buffer, Client &client)
{
	std::vector<std::string> splitCmd = ft_split(buffer, " ");
	bool clientExist = false;
	removeNewlines(splitCmd.back());
	if (splitCmd.size() < 2)
		send_error(client.getFd(), 461, "WHOIS", "ERR_NEEDMOREPARAMS");
	else 
	{
		for (size_t i = 1; i < splitCmd.size(); i++)
		{
			std::map<int, Client *>::iterator it = _server->getClients().begin();
			while (it != _server->getClients().end())
			{
				if (splitCmd[i] == (*it->second).getNickname())
				{
					printMsg(it->second, client, *_server);
					clientExist = true;
				}
				it++;
			}
		}
		if (!clientExist)
			send_error(client.getFd(), 401, "WHOIS", "ERR_NOSUCHNICK");
		else
		{
			std::string response;
			response = ":" + _server->getServerName() + " 318 " + client.getNickname() + " :End of WHOIS list\r\n";
            send(client.getFd(), response.c_str(), response.size(), 0);
		}
	}
}
