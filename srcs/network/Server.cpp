#include "Server.hpp"


Server::Server(ServerConfig config) {
	_listenSocket = new ListenSocket(AF_INET, SOCK_STREAM, 0, config.getPort(), INADDR_ANY, 20);
	_buffer = new char[BUFFER_SIZE];
	_config = config;
	std::cout << "Host:" << config.getHost()<< " listening to new connections on Port:" << ntohs(_listenSocket->getAddress().sin_port) << std::endl;
}

Server::~Server(void) {
	std::cout << "Shutting down server Host:" << _config.getHost()<< " Port:" << ntohs(_listenSocket->getAddress().sin_port) << std::endl;
	close(_listenSocket->getSocket());
	delete _listenSocket;
	delete[] _buffer;
}

ListenSocket* Server::getListenSocket(void) const {
	return _listenSocket;
}

std::vector<int> Server::getActiveConnections(void) const {
	return _activeConnections;
}

Client* Server::listenNewConnections(void) {
	struct sockaddr_in address = _listenSocket->getAddress();
	socklen_t socketLength = static_cast<socklen_t>(sizeof(address));
	int newClientSocket = accept(_listenSocket->getSocket(), (struct sockaddr *)&address, &socketLength);
	if (newClientSocket < 0) {
		perror("error: select(): ");
		return NULL;
	}
	else {
		Client* newClient = new Client(newClientSocket, this);
		_activeConnections.push_back(newClientSocket);
		std::cout << "New socket on fd: " << newClientSocket << std::endl;
		return newClient;
	}
}

void Server::writeSocket(Client *client) {
	int written = write(client->getSock(), vecToStr(client->getWriteBufferVect()).c_str(), client->getWriteBufferSize());
	if (written < 0) {
		closeConnection(client);
		return ;
	}
	if (VERBOSE) 
		printHeader(client->getWriteBufferVect(), written);
	client->clearWriteBuffer(written);
}

void Server::readSocket(Client *client) {
	int sock = client->getSock();
	int received = 0;
	char *readBuffer = _buffer;
	int	bufferSize = BUFFER_SIZE;
	int maxBufferSize = 1024 * 1024 * 6; // 6MB

	if (client->getBodySize()) { //if incomplete request on first read() but we know the body size
		bufferSize += client->getBodySize() - client->getReadBuffer().size();
		bufferSize = (bufferSize > maxBufferSize ? maxBufferSize : bufferSize);
		readBuffer = new char[bufferSize];
	}
	received = read(sock, readBuffer, bufferSize);
	if (received <= 0) {
		if (received < 0)
			std::cout << "read() error" << std::endl;
		if (client->getBodySize())
			delete[] readBuffer;
		closeConnection(client);
		return;
	}
	client->appendReadBuffer(&readBuffer, received);
	if (client->getBodySize())
		delete[] readBuffer;
	if (VERBOSE)
		printHeader(client->getReadBuffer(), received);
	requestToResponseProcess(client, _config);
}

void Server::closeConnection(Client *client) {
	client->kill();
	int sock = client->getSock();
	close(sock);
	std::vector<int>::iterator it_1 = find(_activeConnections.begin(), _activeConnections.end(), sock);
    if (it_1 != _activeConnections.end()) {
        _activeConnections.erase(it_1);
	}
	std::cout << "Closed connection on socket " << sock << std::endl;
}

void Server::printHeader(std::vector<unsigned char> buffer, int max) const {
	std::cout << std::endl;
	for(int i=0; i < max; ++i) {
		if (buffer[i] == '\n' && i + 1 < max && buffer[i + 1] == '\r')
			max = i;
	}
	if (std::cout.write(reinterpret_cast<const char*>(&buffer[0]), max).fail())
		std::cerr << "Failed to write ostream";
	std::cout << std::endl;
}
