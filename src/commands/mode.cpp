#include "../../inc/command.hpp"

void Command::mode(std::string buffer, Client &client)
{
	if (client.getModeSetup() == false)
	{
		client.setModeSetup(true);
		std::vector<std::string> splitCmd = ft_split(buffer, " ");
		rmEndString(splitCmd.back(), 2);

		std::string response = ":" + _server->getServerName() + " MODE " + splitCmd[1] + " " + splitCmd[2] +"\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	}
	else
	{
		std::vector<std::string> args = ft_split(buffer, " ");
		if (args.size() < 2)
		{
			send_error(client.getFd(), 461, "MODE", "Not enough parameters");
			return;
		}

		std::string channelName = args[1];
		if (channelName[0] != '#')
		{
			send_error(client.getFd(), 403, "MODE", "No such channel");
			return;
		}

		Channel *channel = _server->findChannel(channelName);
		if (!channel)
		{
			send_error(client.getFd(), 403, "MODE", "No such channel");
			return;
		}
		if (!channel->clientIsInChannel(client.getFd()))
		{
			send_error(client.getFd(), 442, "MODE", "You're not on that channel");
			return;
		}

		if (args.size() == 2)
		{
			std::string mode = "+";

			if (channel->getTopicRestricted())
				mode += "t";
			if (channel->getInviteOnly())
				mode += "i";
			if (channel->getPasswordProtected())
				mode += "k";
			if (channel->getLimitUsers())
				mode += "l";
			send_response(client.getFd(), "324", client.getNickname(), channelName, mode);
			return;
		}

		std::string mode = args[2];
		bool add = true;
		std::size_t param = 3;

		removeNewlines(mode);
		for (std::size_t i = 0; i < mode.size(); i++)
		{
			switch (mode[i]) 
			{
				case '+':
					add = true;
					break;
				case '-':
					add = false;
					break;
				case 'i':
					if (channel->isOperator(client.getFd()))
						channel->setInviteOnly(add);
					else
						send_error(client.getFd(), 482, "MODE", "You're not a channel operator");
					break;
				case 'k':
					if (channel->isOperator(client.getFd()))
					{
						if (add)
						{
							if (args.size() > param)
							{
								channel->setPasswordProtected(true);
								channel->setPassword(args[param]);
								param++;
							}
							else
							{
								send_error(client.getFd(), 461, "MODE", "Not enough parameters");
								return;
							}
						}
						else
						{
							channel->setPasswordProtected(false);
							channel->setPassword("");
						}
					}
					else
					{
						send_error(client.getFd(), 482, "MODE", "You're not a channel operator");
						return;
					}
					break;
				case 'o':
					if (channel->isOperator(client.getFd()))
					{
						if (args.size() > param)
						{
							std::string nickname = args[param];
							param++;
							Client *newOperator = _server->findClientByNickname(nickname);
							if (!newOperator || !channel->clientIsInChannel(newOperator->getFd()))
							{
								send_error(client.getFd(), 441, "MODE", "They aren't on that channel");
								return;
							}
							if (add)
								channel->addOperator(newOperator);
							else
								channel->removeOperator(newOperator->getFd());
						}
						else
						{
							send_error(client.getFd(), 461, "MODE", "Not enough parameters");
							return;
						}
					}
					else
					{
						send_error(client.getFd(), 482, "MODE", "You're not a channel operator");
						return;
					}
					break;
				case 'l':
					if (channel->isOperator(client.getFd()))
					{
						if (add)
						{
							if (args.size() > param)
							{
								int userLimit = stoi(args[param]);
								param++;
								channel->setLimitUsers(true);
								channel->setMaxUsers(userLimit);
							}
							else
							{
								send_error(client.getFd(), 461, "MODE", "Not enough parameters");
								return;
							}
						}
						else
						{
							channel->setLimitUsers(false);
							channel->setMaxUsers(0);
						}
					}
					else
					{
						send_error(client.getFd(), 482, "MODE", "You're not a channel operator");
						return;
					}
					break;
				case 't':
					if (channel->isOperator(client.getFd()))
						channel->setTopicRestricted(add);
					else
						send_error(client.getFd(), 482, "MODE", "You're not a channel operator");
					break;
				default:
					send_error(client.getFd(), 472, "MODE", "Unknown mode flag");
					return;
			}
		}
	}
}
