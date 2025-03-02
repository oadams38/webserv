#pragma once

# include "../../includes/webserv.hpp"
#include <ctime>
#include <vector>
#include <string>
#include "Server.hpp"

class Server;

class Client {
	public:
		Client(int sock, Server* server);
		Client& operator=(const Client &c);
		int getSock(void) const;
		int getWriteBufferSize(void) const;
		unsigned char* getWriteBuffer(void) const;
		std::vector<unsigned char> getWriteBufferVect(void) const;
		std::vector<unsigned char> getReadBuffer(void) const;
		bool getIsAlive(void) const;
		Server* getServer(void) const ;
		int	getBodySize(void) const ;
		void setBodySize(int size) ;

		void kill(void);
		void appendReadBuffer(char *readBuffer[BUFFER_SIZE], int received);
		void appendWriteBuffer(std::string response);
		void appendWriteBuffer(std::vector<unsigned char> response);
		void appendWriteBuffer(char *response);

		void clearReadBuffer(void);
		void clearWriteBuffer(void);
		void clearWriteBuffer(int size);
		bool	_delete;

	private:
		int _sock;
		std::vector<unsigned char> _readBuffer;
		std::vector<unsigned char> _writeBuffer;
		Server*	_server;
		bool	_isAlive;
		int	_bodySize;
};
