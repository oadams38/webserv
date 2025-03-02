#pragma once

#include "ListenSocket.hpp"
#include "Client.hpp"
#include "../../includes/webserv.hpp"
#include "../Configuration/ServerConfig.hpp"
#define VERBOSE false

class Client;
class ServerConfig;
class ListenSocket;

class Server {
	public:
		Server(ServerConfig config);
		~Server(void);

		ListenSocket * getListenSocket(void) const ;
		std::vector<int> getActiveConnections(void) const;
	
		Client* listenNewConnections(void) ;
		void readSocket(Client *client);
		void writeSocket(Client *client);
		void printHeader(std::vector<unsigned char> buffer, int max) const;

	private:
		void closeConnection(Client *client);

		ServerConfig 			_config;
		ListenSocket *			_listenSocket;
		std::vector<int>		_activeConnections;
		char*					_buffer;
};
