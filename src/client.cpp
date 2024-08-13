#include "../inc/client.hpp"

Client::Client():  _setInfo(0), _modeSetup(false), _endSetup(false), _firstRegister(true), _isOperator(false), _isUnknown(false)
{}

Client::~Client() {}

std::string Client::getName() const
{
	return (_name);
}

bool Client::getFirstRegister()
{
	return (_firstRegister);
}

std::string Client::getNickname() const
{
	return (_nickname);
}

std::string Client::getRealName() const
{
	return (_realName);
}

int Client::getFd()
{
	return (_fd);
}

bool Client::getEndSetup()
{
	return (_endSetup);
}

bool Client::getIsOperator()
{
	return (_isOperator);
}

bool Client::getIsUnknown()
{
	return (_isUnknown);
}

int Client::getSetInfo()
{
	return (_setInfo);
}

bool Client::getModeSetup()
{
	return (_modeSetup);
}

std::vector<std::string> Client::getChannelsIsIn(std::vector<Channel *> channels, int fd)
{
    std::vector<std::string> lstChan;

    std::vector<Channel *>::const_iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->clientIsInChannel(fd))
            lstChan.push_back((*it)->getName());
    }
    return lstChan;
}

void Client::setName(std::string const &str)
{
	_name = str;
}

void Client::setRealName(std::string const &str)
{
	_realName = str;
}

void Client::setNickname(std::string const &str)
{
	_nickname = str;
}

void Client::setFd(int fd)
{
	_fd = fd;
}

void Client::setFalseEndSetup()
{
	_endSetup = false;
}

void Client::setTrueEndSetup()
{
	_endSetup = true;
}

void Client::setFirstRegister(bool state)
{
	_firstRegister = state;
}

void Client::setIsOperator(bool state)
{
	_isOperator = state;
}

void Client::setSetInfo(int step)
{
	_setInfo = step;
}

void Client::setModeSetup(bool state)
{
	_modeSetup = state;
}
