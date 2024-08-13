#include "../inc/command.hpp"
#include "../inc/server.hpp"
#include "../inc/client.hpp"
#include "../inc/channel.hpp"

Command::Command(Server* server, const std::string &commandStr)
    : _server(server), _commandStr(commandStr) {}

Command::~Command() {}

void Command::execute(std::string buffer, Client &client)
{
	std::vector<std::string> splitCmd = ft_split(buffer, " ");
	
	removeNewlines(splitCmd[0]);
	if (splitCmd[0] == "PING")
		ping(buffer, client);
	else if (splitCmd[0] == "MODE")	
		mode(buffer, client);
	else if (splitCmd[0] == "WHOIS")
		whois(buffer, client);
	else if (splitCmd[0] == "QUIT")
		quit(client);
    else if (splitCmd[0] == "motd")
		motd(client);
	else if (splitCmd[0] == "NAMES")
		names(buffer, client);
	else if (splitCmd[0] == "NICK")
		nick(buffer, client);
	else if (splitCmd[0] == "PRIVMSG")
		privmsg(buffer, &client);
	else if (splitCmd[0] == "JOIN")
		join(buffer, &client);
	else if (splitCmd[0] == "lusers")
		lusers(&client);
	else if (splitCmd[0] == "PART")
		part(buffer, &client);
	else if (splitCmd[0] == "TOPIC")
		topic(buffer, &client);
	else if (splitCmd[0] == "KICK")
		kick(buffer, &client, *_server);
	else if (splitCmd[0] == "INVITE")
		invite(buffer, &client);
	else
		std::cout << "Cmd non gerer: " << buffer << std::endl;
}

void Command::send_response(int client, const std::string& code, const std::string& target, const std::string& message, std::string serverName)
{
    std::string response = ":" + serverName + " " + code + " " + target + " :" + message + "\r\n";
    send(client, response.c_str(), response.length(), 0);
}


