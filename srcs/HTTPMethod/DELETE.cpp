#include "../../includes/webserv.hpp"

void	executeDELETE(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf) {
	std::string file = request.getContent();
	
	(void)conf;
	if (access(file.c_str(), F_OK) == -1)
		throw std::runtime_error(E404);

	remove(file.c_str());
	appendToVector(response, "File removed succesfully: ");
	appendToVector(response, request.getContent());
}