#include "../inc/main.hpp"
#include "../inc/command.hpp"

int main(int ac, char *av[])
{
    if (!parseArguments(ac, av))
		return (1);
	
	Server server("irc.csuper.com", std::atoi(av[1]), av[2]);

    server.initServer();
	server.startServer();
	server.closeServer();

    return (0);
}
