#include "../inc/server.hpp"
#include "../inc/command.hpp"
#include "../inc/channel.hpp"

Server::Server() {}

Server::Server(const std::string &name, int port, const std::string &password):
_serverName(name), _serverPort(port), _password(password)
{}

Server::~Server() {}

std::string Server::getServerName() const
{
    return (_serverName);
}

int Server::getServerPort() const
{
    return (_serverPort);
}

std::string Server::getServerPassword() const
{
    return (_password);
}

std::vector<Channel *>& Server::getChannels()
{
    return (_channels);
}

void Server::setName(std::string const &str)
{
	_serverName = str;
}

void Server::setPort(int nb)
{
	_serverPort = nb;
}

/*
** Send an error message to the client
** @param fd: the file descriptor of the client
** @param code: the error code
** @param cmd: the command that failed
** @param err: the error type
*/
void Server::send_error(int fd, int code, std::string cmd, std::string err)
{
	std::string response = ":" + _serverName + " " + itos(code) + " " + cmd + " :" + err + "\r\n";
	send(fd, response.c_str(), response.size(), 0);
}

/*
** Find a fd client in the client's list
** @param fd: the file descriptor to find
** @return true if the fd is found, false otherwise
*/
bool Server::findFdClient(int fd)
{
	std::map<int, Client*>::iterator it = _clients.begin();
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->first == fd && (*it->second).getFirstRegister() == false)
            return true;
    }
    return false;
}

/*
** Find a nickname in the client's list
** @param nickname: the nickname to find
** @return true if the nickname is found, false otherwise
*/
bool Server::findNickname(std::string nickname)
{
	std::map<int, Client*>::iterator it = _clients.begin();
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if ((*it->second).getNickname() == nickname)
            return true;
    }
    return false;
}

/*
** Find a client in the client's list
** @param fd: the file descriptor to find
** @return the client if the fd is found, NULL otherwise
*/
Client *Server::findClient(int fd)
{
	std::map<int, Client*>::iterator it = _clients.begin();
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->first == fd)
            return (it->second);
    }
    return NULL;
}

/*
** Find a client in the client's list by his nickname
** @param nickname: the nickname to find
** @return the client if the nickname is found, NULL otherwise
*/
Client *Server::findClientByNickname(std::string nickname)
{
    removeNewlines(nickname);
    std::map<int, Client*>::iterator it = _clients.begin();
    for (it = _clients.begin(); it != _clients.end(); it++)
    {
        log("NN: " + (*it->second).getNickname() + " | " + nickname, false);
        if ((*it->second).getNickname() == nickname)
            return (it->second);
    }
    return NULL;
}

/*
** Find a channel in the channel's list
** @param name: the name of the channel to find
** @return the channel if the name is found, NULL otherwise
*/
Channel *Server::findChannel(std::string name)
{
    std::vector<Channel *>::iterator it;
    removeNewlines(name);
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if ((*it)->getName() == name)
            return *it;
    }
    return NULL;
}

bool Server::clientIsInChannelServ(std::string channelName, int clientFd)
{
    Channel* channel = findChannel(channelName);
    if (channel == NULL)
        return false;
    const std::map<int, Client*>& clients = channel->getClientsConnected();
    std::map<int, Client*>::const_iterator it;
    for (it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->first == clientFd)
            return true;
    }
    return false;
}

void Server::addClient(int fd, Client *client)
{
    _clients[fd] = client;
}

void Server::onClientConnected(int fd)
{
    Client* newClient = new Client();
    newClient->setFd(fd);

    addClient(fd, newClient);
}

Channel* Server::getChannel(const std::string& channelName) 
{
    for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); ++it) 
    {
        if ((*it)->getName() == channelName) 
            return *it;  
    }
    return NULL;
}

void Server::initServer()
{
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_serverFd == 0)
	{
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
        perror("setsockopt");
        close(_serverFd);
        exit(EXIT_FAILURE);
    }

    if (fcntl(_serverFd, F_SETFL, O_NONBLOCK) == -1)
	{
        perror("fcntl");
        close(_serverFd);
        exit(EXIT_FAILURE);
    }

    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(_serverPort);

    if (bind(_serverFd, (struct sockaddr *)&_address, sizeof(_address)) < 0)
	{
        perror("bind");
        close(_serverFd);
        exit(EXIT_FAILURE);
    }

    if (listen(_serverFd, 10) < 0)
	{
        perror("listen");
        close(_serverFd);
        exit(EXIT_FAILURE);
    }

    _epollFd = epoll_create1(0);
    if (_epollFd == -1)
	{
        perror("epoll_create");
        close(_serverFd);
        exit(EXIT_FAILURE);
    }

    _event.events = EPOLLIN;
    _event.data.fd = _serverFd;

    if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _serverFd, &_event) == -1)
	{
        perror("epoll_ctl");
        close(_serverFd);
        close(_epollFd);
        exit(EXIT_FAILURE);
    }
}

void Server::startServer()
{
    while (1)
    {
        int n = epoll_wait(_epollFd, _events, 10, -1);
        if (n == -1)
        {
            std::cerr << "Error epoll_wait" << std::endl;
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < n; ++i)
        {
            if ((_events[i].events & EPOLLERR) || (_events[i].events & EPOLLHUP))
            {
                std::cerr << "epoll error" << std::endl;
                close(_events[i].data.fd);
                _clients.erase(_events[i].data.fd);
                continue;
            }

            if (_events[i].data.fd == _serverFd)
            {
                int clientFd = accept(_serverFd, (struct sockaddr *)NULL, NULL);
                if (clientFd == -1)
                {
                    perror("accept");
                    continue;
                }
                else
                {
                    std::cout << "New client accepted!" << clientFd << std::endl;
                }

                onClientConnected(clientFd);

                _event.events = EPOLLIN;
                _event.data.fd = clientFd;
                if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, clientFd, &_event) == -1)
                {
                    perror("epoll_ctl");
                    close(clientFd);
                    _clients.erase(clientFd);
                    continue;
                }
                else
                {
                    std::cout << "New client added to poll!" << std::endl;
                }
            }
            else
            {
                int clientFd = _events[i].data.fd;
                char buffer[1024] = {0};
                int read = recv(clientFd, buffer, 1024, 0);
                if (read == 0)
                {
                    if (epoll_ctl(_epollFd, EPOLL_CTL_DEL, clientFd, NULL) == -1)
                    {
                        perror("epoll_ctl");
                    }
                    close(clientFd);
                    _clients.erase(clientFd);
                }
                else if (read < 0)
                {
                    perror("recv");
                    close(clientFd);
                    _clients.erase(clientFd);
                }
                else
                {
                    Client *client = _clients[clientFd];
                    if (client->getEndSetup() == false)
                    {
                        if (connectionSetup(buffer, *client) == -1)
                        {
							std::cerr << "Error setup connection: " << clientFd << std::endl;
                            close(clientFd);
                            _clients.erase(clientFd);
                        }
                    }
                    else
                    {
                        executeCommand(buffer, *client);
                    }
                }
            }
        }
    }
}

int Server::connectionSetup(std::string buffer, Client& client)
{
	std::string tokenExtractLS = buffer.substr(4, 2);
	std::vector<std::string> splitInstruct = ft_split(buffer, "\n");

	if (instructInit(splitInstruct, client) == -1)
        return (-1);
	return (0);
}

void Server::closeServer()
{
	close(_serverFd);
	close(_epollFd);
}

int Server::instructInit(std::vector<std::string> split, Client& client)
{
	std::string response;

	if (client.getSetInfo() != 3)
	{
        for (std::vector<std::string>::iterator it = split.begin(); it != split.end(); ++it)
        {
            if ((*it).substr(0, 4) == "PASS" && !parsePass((*it), client))
                return (-1);
            if ((*it).substr(0, 4) == "NICK" && !parseNick((*it), client))
                return (-1);
            if ((*it).substr(0, 4) == "USER" && !parseUser((*it), client))
                return (-1);
            
            if ((*it).substr(0, 4) == "PASS")
                client.setSetInfo(1);
            if ((*it).substr(0, 4) == "NICK" && client.getSetInfo() == 1)
                client.setSetInfo(2);
            if ((*it).substr(0, 4) == "USER" && client.getSetInfo() == 2)
                client.setSetInfo(3);
            else if ((*it).substr(0, 4) == "USER" && client.getSetInfo() != 2)
            {
                log("Error: need password", true);
		        Server::send_error(client.getFd(), 461, "PASS", "ERR_NEEDMOREPARAMS");
                return (-1);
            }
        }

        if (client.getSetInfo() == 3)
        {
		    response = ":" + getServerName() + " CAP * LS :multi-prefix sasl\r\n";
		    send(client.getFd(), response.c_str(), response.size(), 0);
        }
    }
	else if (split[0].substr(4, 3) == "REQ")
	{
		response = ":" + getServerName() + " CAP * ACK multi-prefi\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);
	}
	else if (split[0].substr(4, 3) == "END")
    {
		response = ":" + getServerName() + " 001 " + client.getNickname() + " :Welcome to the IRC chat Network, " + client.getNickname() + "\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);

		response = ":" + getServerName() + " 002 " + client.getNickname() + " :Your host is " + getServerName() + ", running version 7.7.7\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);

		response = ":" + getServerName() + " 003 " + client.getNickname() + " :This server was created 09/08/2024\r\n";
		send(client.getFd(), response.c_str(), response.size(), 0);

		client.setTrueEndSetup();
	}
    return (0);
}

void Server::executeCommand(const std::string &buffer, Client& client)
{
    std::vector<std::string> splitCmd = ft_split(buffer, " ");

    size_t found = buffer.find('\n');
    if (found == std::string::npos)
    {
        joincmd = joincmd + buffer;
        return ;
    }
    else
        joincmd = joincmd + buffer;
    Command cmd(this, splitCmd[0]);
    cmd.execute(joincmd, client);
    joincmd.erase();
}

std::map<int, Client *>& Server::getClients()
{
    return (_clients);
}
