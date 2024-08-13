#include "../../inc/command.hpp"

void Command::privmsg(std::string buffer, Client *client)
{
	bool isChannel;
	std::string target;
	std::string msg;

	if (buffer.find("#") != std::string::npos)
	{
		isChannel = true;
		std::size_t targetStart = buffer.find("#");
        std::size_t targetEnd = buffer.find(" ", targetStart);
        target = buffer.substr(targetStart, targetEnd - targetStart);
        std::size_t msgStart = buffer.find(":", targetEnd) + 1;
        msg = buffer.substr(msgStart);
	}
	else
	{
		isChannel = false;
		std::size_t targetStart = 8;
        std::size_t targetEnd = buffer.find(" ", targetStart);
        target = buffer.substr(targetStart, targetEnd - targetStart);
        std::size_t msgStart = buffer.find(":") + 1;
        msg = buffer.substr(msgStart);
	}

	if (buffer.find(":") == std::string::npos)
	{
		send_error(client->getFd(), 461, "PRIVMSG", "Not enough parameters");
		return ;
	}
	if (target.empty() || target[0] == ':')
	{
		send_error(client->getFd(), 401, "PRIVMSG", "No nickname given");
		return ;
	}
	if (msg.empty())
	{
		send_error(client->getFd(), 412, "PRIVMSG", "No text to send");
		return ;
	}

	if (isChannel)
	{
		Channel *channel = _server->findChannel(target);
		if (!channel)
		{
			send_error(client->getFd(), 403, "PRIVMSG", "No such channel");
			return ;
		}
		else if (!_server->clientIsInChannelServ(target, client->getFd()))
		{
			send_error(client->getFd(), 404, "PRIVMSG", "Cannot send to channel");
			return ;
		}
		else
		{
			const std::map<int, Client *> &clients = channel->getClientsConnected();
			std::map<int, Client *>::const_iterator it = clients.begin();
			while (it != clients.end())
			{
				if (it->first != client->getFd())
				{
					std::string response = ":" + client->getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
					send(it->first, response.c_str(), response.size(), 0);
				}
				it++;
			}
			return ;
		}
	}
	else
	{
		Client *targetClient = _server->findClientByNickname(target);
		log("Target: " + target, false);
		if (!targetClient)
		{
			log("Target not found", false);
			send_error(client->getFd(), 401, "PRIVMSG", "No such nickname");
			return ;
		}
		else
		{
			std::string response = ":" + client->getNickname() + " PRIVMSG " + target + " :" + msg + "\r\n";
			send(targetClient->getFd(), response.c_str(), response.size(), 0);
			return ;
		}
	}
}
