#pragma once

# define BUFFER_SIZE 1024
# define E400 "400 Bad Request"
# define E404 "404 Not Found"
# define E405 "405 Method Not Allowed"
# define E411 "411 Length Required"
# define E413 "413 Payload Too Large"
# define E500 "500 Internal Server Error"
# define E501 "501 Not Implemented"
# define E505 "505 HTTP Version Not Supported"

# include <iostream>
# include <stdio.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <netinet/in.h>
# include <string>
# include <cstring>
# include <algorithm>
# include <fstream>
# include <vector>
# include <dirent.h>
# include <exception>
# include <map>
# include <deque>
# include <sstream>
# include <stdexcept>
# include "../srcs/HTTPProtocol/HTTPRequest.hpp"
# include "../srcs/HTTPProtocol/HTTPReponse.hpp"
# include "../srcs/network/ASocket.hpp"
# include "../srcs/network/BindSocket.hpp"
# include "../srcs/network/ListenSocket.hpp"
# include "../srcs/network/Server.hpp"
# include "../srcs/network/Client.hpp"
# include "../srcs/Configuration/Config.hpp"
# include "../srcs/Configuration/RouteConfig.hpp"
# include "../srcs/Configuration/ServerConfig.hpp"
# include "template.hpp"

class HTTPRequest;

std::string					strTrim(std::string str, std::string set);
std::string					getTimeStamp();
void						requestToResponseProcess(Client *client, ServerConfig& conf);
std::string					fileToStr(std::string filename);
std::vector<unsigned char>	binaryFileToVector(std::string filename);
bool						isBinaryFile(std::string filename, std::string& type);
std::string					vecToStr(std::vector<unsigned char> vec);
void						executePOST(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf);
void						executeDELETE(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf);
void						executeGET(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf);
void						StrToBinaryFile(std::string filename, std::string& body, std::string boundary);
void						StrToTextFile(std::string filename, std::stringstream& s, std::string boundary, HTTPRequest& request);
std::string					getRedirPath(ServerConfig& conf, std::string method, std::string& url, std::string& content);
void						appendToVector(std::vector<unsigned char>& vec, std::vector<unsigned char> container);
void						appendToVector(std::vector<unsigned char>& vec, std::string container);
void                        callSelect(int maxFd, fd_set *readFds, fd_set *writeFds);
void                        safeWrite(int fd, std::vector<unsigned char> writeVect, int n_tries);
std::vector<unsigned char>  safeRead(int fd);
