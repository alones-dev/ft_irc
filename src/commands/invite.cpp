#include "../../inc/command.hpp"

void Command::invite(std::string buffer, Client *client)
{
	std::vector<std::string> args = ft_split(buffer, " ");
	if (args.size() < 3)
	{
		send_error(client->getFd(), 461, "INVITE", "Not enough parameters");
		return ;
	}

	std::string nickname = args[1];
	std::string channelName = args[2];

	if (nickname.empty() || channelName.empty())
	{
		send_error(client->getFd(), 461, "INVITE", "Not enough parameters");
		return ;
	}

	if (channelName[0] != '#')
	{
		send_error(client->getFd(), 403, "INVITE", "No such channel");
		return ;
	}

	Channel *channel = _server->findChannel(channelName);
	if (!channel)
	{
		send_error(client->getFd(), 403, "INVITE", "No such channel");
		return ;
	}
	if (!channel->clientIsInChannel(client->getFd()))
	{
		send_error(client->getFd(), 442, "INVITE", "You're not in that channel");
		return ;
	}
	if (channel->getInviteOnly() && !channel->isOperator(client->getFd()))
	{
		send_error(client->getFd(), 482, "INVITE", "You're not channel operator");
		return ;
	}

	Client *invitedClient = _server->findClientByNickname(nickname);
	if (!invitedClient)
	{
		send_error(client->getFd(), 401, "INVITE", "No such nickname");
		return ;
	}
	if (channel->clientIsInChannel(invitedClient->getFd()))
	{
		send_error(client->getFd(), 443, "INVITE", "User is already in channel");
		return ;
	}

	channel->addInvitedClient(invitedClient);

	std::string response = ":" + client->getNickname() + " INVITE " + nickname + " :" + channelName;
	send_response(client->getFd(), "341", client->getNickname(), response, _server->getServerName());

	response = ":" + client->getNickname() + " invites you to join " + channelName;
	send_response(invitedClient->getFd(), "NOTICE", nickname, response, _server->getServerName());
}
