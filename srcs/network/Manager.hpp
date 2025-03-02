#pragma once

#include "Server.hpp"
#include <deque>

class Server;

class Manager {
	public:
		Manager(std::deque<Server *> &servers);
		~Manager(void);
		void loop(void);

	private:
		int		checkSockets(void);
		int		processClientQueue(void);
		void	processServerQueue(void);
		void	handleNewConnection(Server *server);

		std::deque<Client *>	_clientQueue;
		std::deque<Server *>	_serverQueue;
		fd_set					_readFds;
		fd_set					_writeFds;
};