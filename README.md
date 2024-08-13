# ft_irc
ft_irc is the first C++ project at 42School (after the c++ Piscine). 

The aim of the project is to create an IRC (Internet Relay Chat) server to manage client connections on a given port. Then, the project must also manage the creation of chat channels, the discussion between clients and other commands we've decided to handle (such as WHOIS, MOTD...).

This is a three-person group project. I personally worked on the client connection to the server, which was then rewritten by my mate. Then I did most of the commands in the project. The rest was shared between my two mates.

## Usage
**1. Clone the repository**
```
git clone https://github.com/alones-dev/ft_irc.git
```
**2. Compile the project**
```
make
```
**3. Enjoy**
```
./ircserv [port] [password]
```
Example:
```
./ircserv 6666 test
```
***➜ to connect to the server, you must use an IRC client such as ``irssi`` (used during project development) or netcat (``nc localhost 6666``).***
