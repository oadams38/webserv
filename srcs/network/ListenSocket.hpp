#pragma once

#include "BindSocket.hpp"

class BindSocket;

class ListenSocket : public BindSocket {
	public:
		ListenSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);
		void doListen(void) ;
		~ListenSocket(void);
	private:
		int _backlog;
		int _listen;
};