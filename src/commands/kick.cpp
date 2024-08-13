#include "../../inc/command.hpp"

void Command::kick(std::string buffer, Client *client, Server &server) 
{
    std::istringstream iss(buffer);
    std::string command, channelName, targetUserNickname, reason;
    
    iss >> command >> channelName >> targetUserNickname;
    std::getline(iss, reason);
    if (!reason.empty() && reason[0] == ' ') 
    {
        reason = reason.substr(1); 
    }
    Channel* channel = server.getChannel(channelName);
    if (!channel) 
    {
        send_error(client->getFd(), 403, "KICK", "ERR_NOSUCHCHANNEL");
        std::cout << "Error: Channel " << channelName << " does not exist." << std::endl;
        return;
    }
    if (!channel->isOperator(client->getFd())) 
    {
        send_error(client->getFd(), 482, "KICK", "ERR_CHANOPRIVSNEEDED");
        std::cout << "Error: You do not have permission to kick users from this channel." << std::endl;
        return;
    }
    int targetUserId = -1;
    const std::map<int, Client*>& clients = channel->getClientsConnected();
    for (std::map<int, Client*>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNickname() == targetUserNickname) 
        {
            targetUserId = it->first;
            break;
        }
    }
    if (targetUserId == -1) 
    {
        send_error(client->getFd(), 442, "KICK", "ERR_NOTONCHANNEL");
        std::cout << "Error: User " << targetUserNickname << " does not exist in channel " << channelName << "." << std::endl;
        return;
    }
    Client* targetUser = channel->getClientsConnected()[targetUserId];
    channel->removeUser(targetUser);

    removeNewlines(reason);
    std::string message = ":" + client->getNickname() + " KICK " + channelName + " " + targetUserNickname + " :" + reason + "\r\n";
    const std::map<int, Client*>& clientsConnected = channel->getClientsConnected();
    std::map<int, Client *>::const_iterator it;
    for (it = clientsConnected.begin(); it != clientsConnected.end(); it++)
        send(it->first, message.c_str(), message.size(), 0);

    std::cout << "User " << targetUserNickname << " has been kicked from channel " << channelName << ". Reason: " << reason << std::endl;
}
