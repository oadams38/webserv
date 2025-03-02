# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>
# include <csignal>
# include "network/ListenSocket.hpp"
# include "network/Server.hpp"
# include "network/Manager.hpp"
# include "network/Client.hpp"
# include "../includes/webserv.hpp"



//initialize the servers vector, containing our running servers. Globally so that we can exit them gracefully with signal handling
std::deque<Server*> servers;

void serverClear(int signum) {
	for (std::deque<Server*>::iterator it = servers.begin(); it < servers.end(); ++it) {
		delete *it;
	}
	servers.clear();
	exit(signum);
}


int main(int argc, char **argv) {

	// Register signal handler for SIGINT
    std::signal(SIGINT, serverClear);


	// Parse the servers from the config file
	Config 						conf;
	std::vector <ServerConfig>	configs;

	try {
		conf.checkParameters(argc, argv);
		conf.parseConfigFile();
		configs = conf.getConfigs();
	} catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << '\n';
		exit(1);
	}

	// Create and populate the server objects with their configs
	for (std::vector<ServerConfig>::iterator it = configs.begin(); it < configs.end(); ++it) {
		try {
			servers.push_back(new Server(*it));
		}
		catch (const std::invalid_argument& e) {
			serverClear(1);
		}
	}

	// Initialize the server operations manager from the servers vector
	Manager serverManager(servers);

	// Run it all
	serverManager.loop();

	serverClear(0);
}
