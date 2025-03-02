#pragma once

#include "ASocket.hpp"

class ASocket;

class BindSocket : public ASocket {
	public:
		BindSocket(int domain, int service, int protocol, int port, u_long interface);
		int connectToNetwork(int socket, struct sockaddr_in address) ;
		~BindSocket(void);
};
