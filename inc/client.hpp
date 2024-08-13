#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "main.hpp"

class Channel;

class Client
{
	private:
		std::string _name;
		std::string _nickname;
		std::string _realName;
		int	_fd;
		int _setInfo;
		bool _modeSetup;
		bool _endSetup;
		bool _firstRegister;
		bool _isOperator;
		bool _isUnknown;
		// bool _onChannel;
	public:
		Client();
		~Client();

		std::string getName() const;
		std::string getNickname() const;
		std::string getRealName() const;
		bool	getEndSetup();
		int	getSetInfo();
		int		getFd();
		bool	getFirstRegister();
		bool getIsOperator();
		bool getIsUnknown();
		bool getModeSetup();
		std::vector<std::string> getChannelsIsIn(std::vector<Channel *> channels, int fd);

		void setName(std::string const &str);
		void setNickname(std::string const &str);
		void setRealName(std::string const &str);
		void setFd(int fd);
		void setFalseEndSetup();
		void setTrueEndSetup();
		void setFirstRegister(bool state);
		void setIsOperator(bool state);
		void setSetInfo(int step);
		void setModeSetup(bool state);
};

#endif
