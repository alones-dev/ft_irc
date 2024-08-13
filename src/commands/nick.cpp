#include "../../inc/command.hpp"

void Command::nick(std::string buffer, Client &client) 
{
    if (buffer.size() < 2)
	{
        log("Error: buffer too short to contain a valid nickname", true);
        send_error(client.getFd(), 432, "NICK", "ERR_ERRONEUSNICKNAME");
        return;
    }

    size_t spacePos = buffer.find(' ', 5);
    std::string new_nick = buffer.substr(5, spacePos - 5);

    if (client.getNickname() == new_nick) 
	{
        log("Error: nickname already exists", true);
        send_error(client.getFd(), 433, "NICK", "ERR_NICKNAMEINUSE");
        return;
    }
	else
	{
        // removeNewlines(new_nick);
		client.setNickname(new_nick);
        send_response(client.getFd(), "NICK", new_nick, "", _server->getServerName());
		std::cout << "new nickname: " << client.getNickname();
	}

}
