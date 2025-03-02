#include "BindSocket.hpp"

BindSocket::BindSocket(int domain, int service, int protocol, int port, u_long interface): 
	ASocket(domain, service, protocol, port, interface) {
		_connection = connectToNetwork(_socket, _address);
		testConnection(_connection);
	}

int BindSocket::connectToNetwork(int socket, struct sockaddr_in address) {
	return bind(socket, (struct sockaddr *)&address, sizeof(address));
}

BindSocket::~BindSocket(void) {}