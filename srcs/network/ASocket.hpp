#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

class ASocket {
	public:
		ASocket(int domain, int service, int protocol, int port, u_long interface);
		virtual int connectToNetwork(int socket, struct sockaddr_in address) = 0;
		void testConnection(int) const ;
		struct sockaddr_in getAddress(void) const;
		int getSocket(void) const;
		int getConnection(void) const;
		virtual ~ASocket(void);
	protected:
		struct sockaddr_in _address;
		int _socket;
		int _connection;
};