#include "../../inc/command.hpp"

void Command::ping(std::string buffer, Client &client)
{	
	std::vector<std::string> splitCmd = ft_split(buffer, " ");
	rmEndString(splitCmd.back(), 2);

	if (splitCmd.size() < 2)
	{
		send_error(client.getFd(), 461, "PING", "Not enough parameters");
		return ;
	}

    std::string response = "PONG :" + splitCmd[1] + "\r\n";
    send(client.getFd(), response.c_str(), response.size(), 0);
}