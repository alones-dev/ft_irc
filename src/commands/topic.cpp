#include "../../inc/command.hpp"

void Command::topic(std::string buffer, Client *client)
{
	std::vector<std::string> args = ft_split(buffer, " ");
	if (args.size() < 2)
	{
		send_error(client->getFd(), 461, "TOPIC", "Not enough parameters");
		return ;
	}

	std::string channelName = args[1];
	std::string topic;

	for (std::size_t i = 2; i < args.size(); ++i)
	{
		topic += args[i];
		if (i + 1 < args.size())
			topic += " ";
	}

	Channel *channel = _server->findChannel(channelName);
	if (!channel)
	{
		send_error(client->getFd(), 403, "TOPIC", "No such channel");
		return ;
	}

	if (topic.empty())
		send_response(client->getFd(), "332", channelName, channel->getTopic(), _server->getServerName());
	else
	{
		if (channel->isOperator(client->getFd()) || !channel->getTopicRestricted())
		{
			channel->setTopic(topic);

			std::map<int, Client *> clients = channel->getClientsConnected();
			std::map<int, Client *>::iterator it = clients.begin();
			while (it != clients.end())
			{
				std::string response = ":" + client->getNickname() + " TOPIC " + channelName + " :" + topic + "\r\n";
				send(it->first, response.c_str(), response.size(), 0);
				it++;
			}
		}
		else
			send_error(client->getFd(), 482, "TOPIC", "You're not channel operator");
	}
}
