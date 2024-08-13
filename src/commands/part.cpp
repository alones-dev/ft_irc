#include "../../inc/command.hpp"

void Command::part(std::string buffer, Client *client)
{
	std::vector<std::string> args = ft_split(buffer, " ");

	if (args.size() < 2)
	{
		send_error(client->getFd(), 461, "PART", "Not enough parameters");
		return ;
	}
	std::vector<std::string> allchannels = ft_split(args[1], ",");

	for (std::size_t i = 0; i < allchannels.size(); ++i)
	{
		std::string channelName = allchannels[i];

		if (channelName.empty() || channelName[0] != '#')
		{
			send_error(client->getFd(), 403, "PART", "No such channel");
			continue ;
		}

		Channel *channel = _server->findChannel(channelName);
		if (!channel)
		{
			send_error(client->getFd(), 403, "PART", "No such channel");
			return ;
		}
		if (!channel->clientIsInChannel(client->getFd()))
		{
			send_error(client->getFd(), 442, "PART", "You're not in that channel");
			return ;
		}
		
		// std::cout << "User leave channel" << std::endl;
		channel->removeClient(client->getFd());
		std::string response = ":" + client->getNickname() + " PART " + channelName + "\r\n";
		std::map<int, Client *> clients = channel->getClientsConnected();
		for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
			send(it->first, response.c_str(), response.size(), 0);

		if (channel->getClientsConnected().empty())
		{
			std::vector<Channel *>::iterator it = _channels.begin();
			while (it != _channels.end())
			{
				if ((*it)->getName() == channelName)
				{
					delete *it;
					_channels.erase(it);
					break ;
				}
				it++;
			}
		}
	}
}
