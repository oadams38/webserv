#include "HTTPReponse.hpp"
#include "HTTPRequest.hpp"
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include "../../includes/webserv.hpp"

char **setEnvCGI(HTTPRequest& request) {
	char **env;
	std::string	tmp;
	int size;
	
	if (request.getMethod() == "GET")
		size = 3;
	else
		size = 4;
	env = new char*[size];
	env[size - 1] = 0;
	env[0] = strdup(tmp.append("METHOD=" + request.getMethod()).c_str());
	if (request.getMethod() != "GET") {
		tmp = "CONTENT-TYPE=" + request.getHeader()["Content-Type"];
		env[1] = new char[tmp.size()];
		env[1] = strcpy(env[1], tmp.c_str()); //leak
		tmp = "CONTENT-LENGTH=" + request.getHeader()["Content-Type"];
		env[2] = new char[tmp.size()];
		env[2] = strcpy(env[2], tmp.c_str()); //leak
	}
	else{
		tmp = "QUERY_STRING=" + request.getQuery();
		env[1] = new char[tmp.size()];
		env[1] = strcpy(env[1], tmp.c_str()); //leak
	}
	return env;
}
void	executeCGI(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf) {
	char **env;
	std::string retVal;
	int fd[2];
	int input[2];
	(void)conf;

	if (pipe(fd) || pipe(input)){
		throw std::runtime_error(E500);} // don't know how to process this kind of error
	if (!access(request.getContent().c_str(), X_OK))
		throw std::runtime_error(E404);
	env = setEnvCGI(request); //needed to send the data to create the response body int the request.getCGIExt() file... still don't know if necessary
	if (fork() == 0){
		dup2(fd[1], STDOUT_FILENO); //get the ouput
		close(fd[1]);
		close(fd[0]);
		dup2(input[0], STDIN_FILENO); //send the input
		close(input[1]);
		close(input[0]);
		std::string content = request.getContent();
		std::string path = request.getCGIPath();
		char *tab[3] = {(char *)path.c_str(), (char*)content.c_str(), 0};
		execve(path.c_str(), tab, env);
		perror("error: execve(): ");
		throw std::runtime_error(E500);
		exit(1);
	}
	close(fd[1]);
	if (request.getMethod() == "POST"){ //send the input
		close(input[0]);
		try {
			safeWrite(input[1], request.getBody(), 5);
		}
		catch (std::exception &e) {
			throw std::runtime_error(E500);
			close(input[1]);
			for (int i = 0; env[i]; i++)
				delete[] env[i];
			delete[] env;
			throw std::runtime_error(E500);
		}
		close(input[1]);
	}
	response.clear();
	try {
		std::vector<unsigned char> readVect = safeRead(fd[0]);
		response.insert(response.end(), readVect.begin(), readVect.end());
	}
	catch (std::exception &e) {
		for (int i = 0; env[i]; i++)
			delete[] env[i];
		delete[] env;
		throw ((std::runtime_error&)e);
	}
	close(fd[0]);
	wait(0);
	for (int i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;
}

void	executeRequest(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf){
	if (request.getMethod() != "DELETE" && request.getContent().find(request.getCGIExt()) < request.getContent().size())
		executeCGI(request, response, conf);
	else if (request.getMethod() == "GET")
		executeGET(request, response, conf);
	else if (request.getMethod() == "POST")
		executePOST(request, response, conf);
	else if (request.getMethod() == "DELETE")
		executeDELETE(request, response, conf);
}

void	requestToResponseProcess(Client *client, ServerConfig& conf) {
	std::vector<unsigned char> final;
	std::vector<unsigned char> repBody;

	try {
		HTTPRequest request(client, conf);
		executeRequest(request, repBody, conf);
		HTTPReponse response(repBody, request);
		final = response.getFinal();
	}
	catch (std::exception& e) {
		if (e.what() == (std::string)"incomplete")
			return;
		HTTPReponse error((std::string)e.what(), conf);
		client->appendWriteBuffer((std::vector<unsigned char>)error.getFinal());
	}
	client->appendWriteBuffer(final);
	client->clearReadBuffer();
}