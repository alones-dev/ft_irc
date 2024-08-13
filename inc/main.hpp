#ifndef MAIN_HPP
#define MAIN_HPP

/*****************************************************************************/
/*                                  INCLUDES                                 */
/*****************************************************************************/

#include <string>
#include <cstring>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <limits.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>


#include "server.hpp"
#include "client.hpp"
#include "channel.hpp"

/*****************************************************************************/
/*                                  COLORS                                   */
/*****************************************************************************/

#define RED		"\033[0;31m"
#define GREEN	"\033[0;32m"
#define YELLOW	"\033[0;33m"
#define ORANGE "\033[38;5;208m"
#define BLUE	"\033[0;34m"
#define MAGENTA	"\033[0;35m"
#define CYAN	"\033[0;36m"
#define WHITE	"\033[0;37m"
#define BOLD       "\033[1m"
#define UNDERLINE  "\033[4m"
#define ITALIC     "\033[3m"
#define RESET      "\033[0m"


/*****************************************************************************/
/*                                  CLASSES                                  */
/*****************************************************************************/


/*****************************************************************************/
/*                                  TEMPLATES                                */
/*****************************************************************************/

/*****************************************************************************/
/*                                  FONCTIONS                                */
/*****************************************************************************/

/* utils.cpp */
std::vector<std::string> ft_split(std::string str, std::string sep);
void log(std::string msg, bool err);
void rmEndString(std::string &str, size_t n);
void removeNewlines(std::string &str);
bool isPrintable(std::string str);
bool parseArguments(int ac, char **av);
std::string itos(int nb);
bool isInSet(std::string str, std::string set);
void removeNewlines(std::string &str);
std::string ft_trim(std::string str);
std::string joinVector(std::vector<std::string> str, const std::string &separator);
int stoi(std::string &str);
void removeAfterWord(std::string &str);

/* parsing.cpp */
bool parseArguments(int ac, char **av);

#endif
