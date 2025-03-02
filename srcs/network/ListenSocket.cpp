#include "ListenSocket.hpp"

ListenSocket::ListenSocket(int domain, int service, int protocol, int port, u_long interface, int backlog): 
	BindSocket(domain, service, protocol, port, interface) {
	_backlog = backlog;
	doListen();
	testConnection(_listen);
}

void ListenSocket::doListen(void) {
	_listen = listen(_socket, _backlog);
}

ListenSocket::~ListenSocket(void) {}