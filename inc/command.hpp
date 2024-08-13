#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "main.hpp"
#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

class Command: public Server
{
	private:
		Server* _server;
    	std::string _commandStr;
	public:
		Command(Server* server, const std::string &commandStr);
		~Command();

		void ping(std::string buffer, Client &client);
		void mode(std::string buffer, Client &client);
		void quit(Client &client);
		void whois(std::string buffer, Client &client);
		void send_response(int client, const std::string& code, const std::string& target, const std::string& message, std::string serverName);
		void motd(Client &client);
		
		void execute(std::string buffer, Client &client);
		void names(std::string buffer, Client &client);
		void nick(std::string buffer, Client &client);
		void privmsg(std::string buffer, Client *client);
		void join(std::string buffer, Client *client);
		void topic(std::string buffer, Client *client);
		void lusers(Client *client);
		void part(std::string buffer, Client *client);
		void invite(std::string buffer, Client *client);
		void kick(std::string buffer, Client *client, Server &server);
};

#endif
