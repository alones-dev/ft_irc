#include "../../inc/command.hpp"

void Command::join(std::string buffer, Client *client)
{
	buffer.erase(0, 5);
	if (buffer.empty())
	{
		send_error(client->getFd(), 461, "JOIN", "Not enough parameters");
		return ;
	}


	std::string channels = buffer;
	std::string keys = "";
	if (buffer.find(' ') != std::string::npos)
	{
		channels = buffer.substr(0, buffer.find(' '));
		keys = buffer.substr(buffer.find(' ') + 1);
	}

	std::vector<std::string> splitChannels = ft_split(channels, ",");
	std::vector<std::string> splitKeys = ft_split(keys, ",");

	for (std::size_t i = 0; i < splitChannels.size(); ++i)
	{
		std::string channelName = splitChannels[i];
		std::string key = (i < splitKeys.size()) ? splitKeys[i] : "";

		if (channelName[0] != '#')
		{
			send_error(client->getFd(), 403, "JOIN", "No such channel");
			return ;
		}

		Channel *channel = _server->findChannel(channelName);

		std::vector<Channel *>& allChannel = _server->getChannels();
		if (!channel)
		{
			removeNewlines(channelName);
			channel = new Channel(channelName);
			channel->addOperator(client);
			client->setIsOperator(true);
			if (key != "")
			{
				channel->setPassword(key);
				channel->setPasswordProtected(true);
			}
			allChannel.push_back(channel);
		}

		if (channel->getPasswordProtected() && key != channel->getPassword())
		{
			send_error(client->getFd(), 475, "JOIN", "Cannot join channel (+k)");
			return ;
		}

		if (channel->getInviteOnly() && !channel->clientIsInChannelInvited(client->getFd()))
		{
			send_error(client->getFd(), 473, "JOIN", "Cannot join channel (+i)");
			return ;
		}

		if (channel->getLimitUsers() && channel->getClientsConnected().size() >= channel->getMaxUsers())
		{
			send_error(client->getFd(), 471, "JOIN", "Cannot join channel (+l)");
			return ;
		}

		if (!channel->clientIsInChannel(client->getFd()))
		{
			channel->addClient(client);

			std::string response = ":" + client->getNickname() + " JOIN " + channelName + "\r\n";
			send(client->getFd(), response.c_str(), response.size(), 0);

			std::map<int, Client *> clients = channel->getClientsConnected();
			std::map<int, Client *>::iterator it = clients.begin();
			while (it != clients.end())
			{
				if (it->first != client->getFd())
				{
					std::string response = ":" + client->getNickname() + " JOIN " + channelName + "\r\n";
					send(it->first, response.c_str(), response.size(), 0);
				}
				it++;
			}
		}
	}
}
