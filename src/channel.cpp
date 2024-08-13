#include "../inc/channel.hpp"

Channel::Channel()
{
}

Channel::Channel(std::string name) : _name(name), _password(""), _topic(""), _inviteOnly(false), _topicRestricted(false), _passwordProtected(false), _limitUsers(false), _maxUsers(0)
{
}

Channel::~Channel()
{
}

std::string Channel::getName() const
{
	return (_name);
}

std::map<int, Client *> Channel::getClientsConnected()
{
	return (_clientsConnected);
}

std::string Channel::getTopic()
{
	return (_topic);
}

bool Channel::getTopicRestricted()
{
	return (_topicRestricted);
}

bool Channel::getInviteOnly()
{
	return (_inviteOnly);
}

bool Channel::getLimitUsers()
{
	return (_limitUsers);
}

bool Channel::getPasswordProtected()
{
	return (_passwordProtected);
}

std::string Channel::getPassword()
{
	return (_password);
}

std::size_t Channel::getMaxUsers()
{
	return (_maxUsers);
}

void Channel::setName(std::string name)
{
	_name = name;
}

void Channel::setTopicRestricted(bool state)
{
	_topicRestricted = state;
}

void Channel::setInviteOnly(bool state)
{
	_inviteOnly = state;
}

void Channel::setPassword(std::string password)
{
	_password = password;
}

void Channel::setLimitUsers(bool state)
{
	_limitUsers = state;
}

void Channel::setPasswordProtected(bool state)
{
	_passwordProtected = state;
}

void Channel::setMaxUsers(std::size_t maxUsers)
{
	_maxUsers = maxUsers;
}

void Channel::setTopic(std::string topic)
{
	_topic = topic;
}

void Channel::addClient(Client *client)
{
	_clientsConnected[client->getFd()] = client;
}

void Channel::addInvitedClient(Client *client)
{
	_invitedClients[client->getFd()] = client;
}

void Channel::addOperator(Client *client)
{
	_operatorsClients[client->getFd()] = client;
}

void Channel::removeOperator(int fd)
{
	std::map<int, Client *>::iterator it = _operatorsClients.begin();
	while (it != _operatorsClients.end())
	{
		if (it->first == fd)
		{
			_operatorsClients.erase(it);
			break;
		}
		it++;
	}
}

void Channel::removeClient(int fd)
{
	std::map<int, Client *>::iterator it = _clientsConnected.begin();
	while (it != _clientsConnected.end())
	{
		if (it->first == fd)
		{
			_clientsConnected.erase(it);
			break;
		}
		it++;
	}

	std::map<int, Client *>::iterator it2 = _operatorsClients.begin();
	while (it2 != _operatorsClients.end())
	{
		if (it2->first == fd)
		{
			_operatorsClients.erase(it2);
			break;
		}
		it2++;
	}

	std::map<int, Client *>::iterator it3 = _invitedClients.begin();
	while (it3 != _invitedClients.end())
	{
		if (it3->first == fd)
		{
			_invitedClients.erase(it3);
			break;
		}
		it3++;
	}
}

bool Channel::clientIsInChannel(int fd)
{
	std::map<int, Client *>::iterator it = _clientsConnected.begin();
	while (it != _clientsConnected.end())
	{
		if (it->first == fd)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::clientIsInChannelInvited(int fd)
{
	std::map<int, Client *>::iterator it = _invitedClients.begin();
	while (it != _invitedClients.end())
	{
		if (it->first == fd)
			return (true);
		it++;
	}
	return (false);
}

bool Channel::isOperator(int fd)
{
	std::map<int, Client *>::iterator it = _operatorsClients.begin();
	while (it != _operatorsClients.end())
	{
		if (it->first == fd)
			return (true);
		it++;
	}
	return (false);
}

int Channel::getNumClients() 
{
    return clients_.size();
}

Client** Channel::getClients() const
{
    Client** clients = new Client*[clients_.size()];
    for (size_t i = 0; i < clients_.size(); i++) 
	{
        clients[i] = clients_[i];
    }
    return clients;
}

void Channel::removeUser(Client* client) 
{
    for (std::map<int, Client*>::iterator it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) 
    {
        if (it->second == client) 
        {
            _clientsConnected.erase(it);
            break;
        }
    }
}

void Channel::notifyKick(const std::string& targetUserNickname, const std::string& reason) 
{
    std::string message = ":" + getName() + " KICK " + getName() + " " + targetUserNickname + " :" + reason + "\r\n";
    for (std::map<int, Client*>::iterator it = _clientsConnected.begin(); it != _clientsConnected.end(); ++it) 
    {
        std::cout << message << std::endl;
    }
} 


