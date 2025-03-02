#include "Manager.hpp"

Manager::Manager(std::deque<Server *> &servers): _serverQueue(servers) {}

Manager::~Manager(void) {}

void Manager::loop(void) {
	while (true) {
		if (checkSockets() == 0) {
			if (processClientQueue() != 0)
				processServerQueue();
		};
	}
}

int Manager::checkSockets(void) { 
	int sock;
	int maxFd = 0;

	//clear fd_sets before every select
	FD_ZERO(&_readFds);
	FD_ZERO(&_writeFds);
	//Add listening sockets to read fd_set
	for (std::deque<Server *>::iterator server = _serverQueue.begin(); server < _serverQueue.end(); ++server) {
		sock = (*server)->getListenSocket()->getSocket();
		FD_SET(sock, &_readFds);
		if (maxFd < sock)
			maxFd = sock;
	}
	//Add client sockets to either read fd_set or write fd_set
	for (std::deque<Client*>::iterator it = _clientQueue.begin(); it != _clientQueue.end(); ++it) {
		if ((*it)->getIsAlive()) {
			sock = (*it)->getSock();
			if ((*it)->getWriteBufferSize())
				FD_SET(sock, &_writeFds);
			else
				FD_SET(sock, &_readFds);
			if (maxFd < sock)
				maxFd = sock;
		}
	}
	try {
		callSelect(maxFd, &_readFds, &_writeFds);
	}
	catch (std::exception &e) {
		return -1;
	}
	return 0;
}

int Manager::processClientQueue(void) {
	int			queueSize = _clientQueue.size();
	Client*		client;

	//Stop if you have cycled through the whole queue
	while (queueSize--) {
		client = _clientQueue.front();
		_clientQueue.pop_front();
		if (client->getIsAlive()) {
			_clientQueue.push_back(client);
			if (FD_ISSET(client->getSock(), &_readFds)) {
				client->getServer()->readSocket(client);	//read operation execution
				return 0;
			}
			else if (FD_ISSET(client->getSock(), &_writeFds)) {
				client->getServer()->writeSocket(client);	//write operation execution
				return 0;
			}
		}
		else
			delete client;
	}
	return 1;
}

void Manager::processServerQueue(void) {
	int			queueSize = _serverQueue.size();
	Server*		server;
	while (queueSize--) {
		server = _serverQueue.front();
		_serverQueue.pop_front();
		_serverQueue.push_back(server);
		if (FD_ISSET(server->getListenSocket()->getSocket(), &_readFds)) {
			handleNewConnection(server);
			return;
		}
	}
}


void Manager::handleNewConnection(Server *server) {
	Client* newClient = server->listenNewConnections();
	if (!newClient) //error
		return;
	else //if client connection was done by server,
		_clientQueue.push_back(newClient);
}

