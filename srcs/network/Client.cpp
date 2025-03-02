#include "Client.hpp"

Client::Client(int sock, Server* server): _sock(sock), _server(server) {
	_isAlive = true;
	_delete = false;
	_bodySize = 0;
}

Client& Client::operator=(const Client &c) {
	_sock = c.getSock();
	_readBuffer = c.getReadBuffer();
	_writeBuffer = c.getWriteBufferVect();
	_isAlive = c.getIsAlive();
	_server = c.getServer();
	_bodySize = c.getBodySize();
	return *this;
}

int Client::getSock(void) const {
	return _sock;
}
std::vector<unsigned char> Client::getReadBuffer(void) const {
	return _readBuffer;
}
std::vector<unsigned char> Client::getWriteBufferVect(void) const {
	return _writeBuffer;
}
unsigned char * Client::getWriteBuffer(void) const {
	return (unsigned char *)_writeBuffer.data();
}
int Client::getWriteBufferSize(void) const {
	return _writeBuffer.size();
}
bool Client::getIsAlive(void) const {
	return _isAlive;
}
Server* Client::getServer(void) const {
	return _server;
}
int	Client::getBodySize(void) const {
	return _bodySize;
}
void Client::setBodySize(int size) {
	_bodySize = size;
}

void Client::kill(void) {
	_isAlive = false;
}

void Client::appendReadBuffer(char *readBuffer[BUFFER_SIZE], int received) {
	_readBuffer.insert(_readBuffer.end(), *readBuffer, *readBuffer + received);
}

void Client::appendWriteBuffer(std::string response){
	appendToVector(_writeBuffer, response);
}
void Client::appendWriteBuffer(std::vector<unsigned char> response){
	appendToVector(_writeBuffer, response);
}
void Client::appendWriteBuffer(char *response){
	appendToVector(_writeBuffer, response);
}

void Client::clearReadBuffer(void) {
	_readBuffer.clear();
}
void Client::clearWriteBuffer(void) {
	_writeBuffer.clear();
}
void Client::clearWriteBuffer(int size) {
	_writeBuffer.erase(_writeBuffer.begin(), _writeBuffer.begin() + size);
}