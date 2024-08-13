#ifndef SERVER_HPP
#define SERVER_HPP

#include "main.hpp"

class Client;
class Channel;

class Server
{
    protected:
        std::string _serverName;
        int _serverPort;
        std::string _password;
        int _serverFd;
        int _epollFd;
        std::string joincmd;

        struct sockaddr_in	_address;
		struct epoll_event	_event;
        struct epoll_event _events[10];

        std::map<int, Client *> _clients;
        std::vector<Channel *> _channels;
    public:
        Server();
        Server(const std::string &name, int port, const std::string &password);
        ~Server();

        std::string getServerName() const;
        int getServerPort() const;
        std::string getServerPassword() const;
        std::map<int, Client *>& getClients();
        std::vector<Channel *>& getChannels();

        void setName(std::string const &str);
        void setPort(int nb);

        void initServer();
        void startServer();

        int connectionSetup(std::string buffer, Client& client);

        int instructInit(std::vector<std::string> split, Client& client);


        void closeServer();
        void send_error(int fd, int code, std::string cmd, std::string err);


		bool parseCAP_LS(std::string buffer, Client &client);
		bool parsePass(std::string buffer, Client &client);
		bool parseNick(std::string buffer, Client &client);
		bool parseUser(std::string buffer, Client &client);

		bool findFdClient(int fd);
		bool findNickname(std::string nickname);
		Client *findClient(int fd);
        Client *findClientByNickname(std::string nickname);

        void executeCommand(const std::string &buffer, Client& client);
        Channel *findChannel(std::string name);
        bool clientIsInChannelServ(std::string channelName, int clientFd);

        void addClient(int fd, Client *client);
        void onClientConnected(int fd);

        Channel* getChannel(const std::string& channelName);
};

#endif
