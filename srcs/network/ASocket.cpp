#include "ASocket.hpp"

ASocket::ASocket(int domain, int service, int protocol, int port, u_long interface) {
	_address.sin_family = domain;
	_address.sin_port = htons(port);
	_address.sin_addr.s_addr = htonl(interface);
	_socket = socket(domain, service, protocol);
	testConnection(_socket);
}

void ASocket::testConnection(int item) const {
	if (item < 0) {
		perror("Failed to connect: ");
		throw std::invalid_argument("Port or socket already in use");
	}
}

struct sockaddr_in ASocket::getAddress(void) const {
	return _address;
}
int ASocket::getSocket(void) const {
	return _socket;
}
int ASocket::getConnection(void) const {
	return _connection;	
}

ASocket::~ASocket(void) {
	close(_socket);
}