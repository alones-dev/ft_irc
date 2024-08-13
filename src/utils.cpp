#include "../inc/main.hpp"

/*
** Split a string into a vector of strings
** @param str: the string to split
** @param sep: the separator for the split
** @return a vector of strings
*/
std::vector<std::string> ft_split(std::string str, std::string sep)
{
	std::vector<std::string> result;
	size_t start = 0;
	size_t end = 0;

	while ((end = str.find(sep, start)) != std::string::npos)
	{
		result.push_back(str.substr(start, end - start));
		start = end + sep.length();
	}
	result.push_back(str.substr(start));
	
	return result;
}

/*
** Print a message to the console
** @param msg: the message to print
** @param err: print in stderr or stdout
*/
void log(std::string msg, bool err)
{
	if (err)
		std::cerr << msg << std::endl;
	else
		std::cout << msg << std::endl;
}

/*
** Check if a string is printable 
** @param str: the string to check
** @return true if the string is printable, false otherwise
*/
bool isPrintable(std::string str)
{
	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (!std::isprint(str[i]))
			return false;
	}

	return true;
}

/*
** remove carac from a string
** @param str: the string to check
** @param n: number of carac to remove
*/
void rmEndString(std::string &str, size_t n)
{
	if (!str.empty() && str.length() >= n)
		str.erase(str.length() - n);
}

/*
** remove newlines from a string
** @param str: the string to check
*/
void removeNewlines(std::string &str)
{
    std::string::iterator end_pos = std::remove(str.begin(), str.end(), '\r');
    str.erase(end_pos, str.end());

    end_pos = std::remove(str.begin(), str.end(), '\n');
    str.erase(end_pos, str.end());
}

/*
** Convert an int to a string
** @param nb: the int to convert
** @return the int converted to a string
*/
std::string itos(int nb)
{
	std::stringstream ss;

	ss << nb;
	
	return ss.str();
}

/*
** Check if a string character is in a set
** @param str: the string to check
** @param set: the set of characters
** @return true if the string is in the set, false otherwise
*/
bool isInSet(std::string str, std::string set)
{
	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (set.find(str[i]) == std::string::npos)
			return false;
	}
	return true;
}

/*
** Remove leading and trailing whitespaces from a string
** @param str: the string to trim
** @return the trimmed string
*/
std::string ft_trim(std::string str)
{
	std::size_t first = str.find_first_not_of(" ");
	if (first == std::string::npos)
		return str;
	
	std::size_t last = str.find_last_not_of(" ");
	return str.substr(first, (last - first + 1));
}


/*
*** Join string of caractere from a vector
*/
std::string joinVector(std::vector<std::string> str, const std::string &separator)
{
    std::string result;

    if (str.empty())
        return result;
    for (std::vector<std::string>::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (it != str.begin())
            result += separator;
        result += *it;
    }
    return (result);
}

/*
** Convert a string to an int
** @param str: the string to convert
** @return the string converted to an int
*/
int stoi(std::string &str)
{
	std::stringstream ss(str);
	int nb;

	ss >> nb;

	return (nb);
}

/* Remove all bad characters after a word 
** @param str: the string to check
*/
void removeAfterWord(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (!isprint(str[i]))
		{
			str.erase(i, str.length() - i);
		}
	}
}
