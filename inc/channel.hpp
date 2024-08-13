#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "main.hpp"

class Channel
{
	private:
		std::string _name;
		std::string _password;
		std::string _topic;
		bool _inviteOnly;
		bool _topicRestricted;
		bool _passwordProtected;
		bool _limitUsers;
		std::size_t _maxUsers;
		std::map<int, Client *> _clientsConnected;
		std::map<int, Client *> _invitedClients;
		std::map<int, Client *> _operatorsClients;
		std::vector<Client*> clients_;
	public:
		Channel();
		Channel(std::string name);
		~Channel();

		std::string getName() const;
		std::map<int, Client *> getClientsConnected();
		std::string getTopic();
		std::string getPassword();
		bool getTopicRestricted();
		bool getInviteOnly();
		bool getLimitUsers();
		bool getPasswordProtected();
		std::size_t getMaxUsers();

		void setName(std::string name);
		void setTopic(std::string topic);
		void setPassword(std::string password);
		void setTopicRestricted(bool state);
		void setInviteOnly(bool state);
		void setLimitUsers(bool state);
		void setPasswordProtected(bool state);
		void setMaxUsers(std::size_t maxUsers);

		void addClient(Client *client);
		void addInvitedClient(Client *client);
		void addOperator(Client *client);
		void removeOperator(int fd);
		void removeClient(int fd);
		bool clientIsInChannel(int fd);
		bool clientIsInChannelInvited(int fd);
		bool isOperator(int fd);
		
		int getNumClients();

		Client** getClients() const;
		void notifyKick(const std::string &kickedUser, const std::string &reason);
        void removeUser(Client* client);
};

#endif
