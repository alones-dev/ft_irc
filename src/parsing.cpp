#include "../inc/main.hpp"
#include "../inc/server.hpp"

/*
** Parse the arguments of the program
** @param ac: the number of arguments
** @param av: the arguments list
** @return true if the arguments are valid, false otherwise
*/
bool parseArguments(int ac, char **av)
{
	if (ac != 3)
	{
		log("Error: ./ircserv [port] [password]", true);
		return false;
	}

	int port = std::atoi(av[1]);
	if (port < 6660 || port > 6669)
	{
		log("Error: port range is 6660-6669", true);
		return false;
	}

	if (!isPrintable(av[2]))
	{
		log("Error: invalid password", true);
		return false;
	}
	
	return true;
}

/*
** Parse the CAP LS informations
** @param buffer: the buffer to parse
** @param client: the client object
** @return true if the informations are valid, false otherwise
*/
bool Server::parseCAP_LS(std::string buffer, Client &client)
{
	std::vector<std::string> split = ft_split(buffer, "\n");

	if (split.size() - 1 != 4)
	{
		log("Error: bad informations", true);
		return false;
	}

	if (!parsePass(split[1], client))
		return false;

	if (!parseNick(split[2], client))
		return false;

	if (!parseUser(split[3], client))
		return false;
	
	return true;
}

/*
** Parse the password in the CAP LS informations
** @param buffer: the line of CAP LS with the password informations
** @param client: the client object
** @return true if the informations are valid, false otherwise
*/
bool Server::parsePass(std::string buffer, Client &client)
{
	std::string password = buffer.substr(5, buffer.length() - 5);
	removeAfterWord(password);
	// std::cout << "password: " << password << std::endl;
	if (buffer.compare(0, 5, "PASS ") != 0)
	{
		// log("Error: bad informations", true);
		return false;
	}
	else if (password.length() <= 0)
	{
		log("Error: need password", true);
		Server::send_error(client.getFd(), 461, "PASS", "ERR_NEEDMOREPARAMS");
		return false;
	
	}
	else if (password.compare(0, _password.length() + 1, _password) == 0)
	{
		if (findFdClient(client.getFd()))
		{
			log("Error: you are already connected", true);
			Server::send_error(client.getFd(), 462, "PASS", "ERR_ALREADYREGISTRED");
			return false;
		}
		else
		{

			client.setFirstRegister(false);
			log("good password", false);
			return true;
		}
	}
	else
	{
		log("Error: bad password", true);
		Server::send_error(client.getFd(), 464, "PASS", "ERR_PASSWDMISMATCH");
		return false;
	}	
	
	return true;
}

/*
** Parse the nickname
** @param buffer: the buffer to parse containing the nickname
** @param client: the client object
** @return true if the informations are valid, false otherwise
*/
bool Server::parseNick(std::string buffer, Client &client)
{
	std::string nickname = buffer.substr(5);

	if (buffer.compare(0, 5, "NICK ") != 0)
	{
		log("Error: bad informations", true);
		return false;
	}
	else if (nickname.length() <= 0)
	{
		log("Error: need nickname", true);
		Server::send_error(client.getFd(), 431, "NICK", "ERR_NONICKNAMEGIVEN");
		return false;
	}
	else if (isInSet(nickname, " ,:;@+!*#&$"))
	{
		log("Error: invalid nickname", true);
		Server::send_error(client.getFd(), 432, "NICK", "ERR_ERRONEUSNICKNAME");
		return false;
	}
	else if ((nickname[0] < 65 && nickname[0] > 90) || (nickname[0] < 97 && nickname[0] > 122))
	{
		log("Error: invalid nickname", true);
		Server::send_error(client.getFd(), 432, "NICK", "ERR_ERRONEUSNICKNAME");
		return false;
	}
	else if (findNickname(nickname))
	{
		log("Error: nickname already exist", true);
		Server::send_error(client.getFd(), 433, "NICK", "ERR_NICKNAMEINUSE");
		return false;
	}
	else
	{
		removeNewlines(nickname);
		client.setNickname(nickname);
	}

	return true;
}

/*
** Parse the username
** @param buffer: the buffer to parse containing the username
** @param client: the client object
** @return true if the informations are valid, false otherwise
*/
bool Server::parseUser(std::string buffer, Client &client)
{
	std::string username = buffer.substr(5);
	if (buffer.compare(0, 5, "USER ") != 0)
	{
		log("Error: bad informations", true);
		return false;
	}
	if (username.find_first_of(" ", 0) != std::string::npos)
	{
		username.erase(username.find_first_of(" ", 0));
	}
	if (username.length() <= 0)
	{
		log("Error: need username", true);
		Server::send_error(client.getFd(), 461, "USER", "ERR_NEEDMOREPARAMS");
		return false;
	}
	else
	{
		removeNewlines(username);
		client.setName(username);
	}

	std::string realname = buffer.substr(buffer.find_first_of(":", 0) + 1);
	if (realname.length() <= 0)
	{
		log("Error: need realname", true);
		Server::send_error(client.getFd(), 461, "USER", "ERR_NEEDMOREPARAMS");
		return false;
	}
	else
	{
		removeNewlines(realname);
		client.setRealName(realname);
	}

	return true;
}
