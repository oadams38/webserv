#include "../../includes/webserv.hpp"

void makeAutoindex(std::string local, std::vector<unsigned char>& body) {
	DIR * dir = opendir(local.c_str());
	struct dirent * content;\
	std::string newFile = "<!DOCTYPE html>\r\n<html>\r\n\
    <head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\r\n\
    <title>Autoindex " + local + "</title>\r\n\
    <style>\r\n\
        /* Style for the whole page */\r\n\
        body {\r\n\
            display: flex;\r\n\
            height: 100vh;\r\n\
            margin: 0;\r\n\
            background-color: #000000; /* Light gray background */\r\n\
            color: #333; /* Dark gray text */\r\n\
            font-family: system-ui, -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif; /* Change font */\r\n\
        }\r\n\
\r\n\
        /* Style for the h1 element */\r\n\
        h1 {\r\n\
            font-size: 2em; /* Increase font size */\r\n\
            color: #a7a7a7; /* Light red color */\r\n\
            margin: 0;\r\n\
        }\r\n\
\r\n\
        /* Style for the paragraph element */\r\n\
        p {\r\n\
            font-size: 1.2em; /* Larger font size */\r\n\
            color: rgb(255, 255, 255); /* Medium gray color */\r\n\
        }\r\n\
    </style>\r\n\
	</head>\r\n<body>\r\n\t<div>\r\n\t\t<h1>" + local + " content:</h1>";

	while (1){
		content = readdir(dir);
		if (!content)
			break;
		newFile.append("\t\t<p>");
		if (content->d_type != 4)
			newFile.append("<i>");
		else
			newFile.append("<b>");
		newFile.append(content->d_name);
		if (content->d_type != 4)
			newFile.append("</i>");
		else
			newFile.append("</b>");
		newFile.append("</p>\r\n");
	}
	newFile.append("\t</div>\r\n</body>\r\n</html>\r\n");
	closedir(dir);
	appendToVector(body, newFile);
}

void	executeGET(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf) {
	if (request.getContent().find("autoindex") <= request.getContent().size()){
		std::string local = request.getContent().substr(0, request.getContent().find("autoindex"));
		return makeAutoindex(local, response);}
	if (isBinaryFile(request.getContent(), request.getHeader()["Content-Type"])){
		std::vector<unsigned char> vec = binaryFileToVector(request.getContent());
		response = vec;
	}
	else if (!access(request.getContent().c_str(), F_OK))
		appendToVector(response, fileToStr(request.getContent()));
	else if (request.getContent().find("http") < request.getContent().size()){
		request.setStatus("302");
		request.addToHeader("Location", request.getContent());
	}

	if (response.size() > conf.getClientMaxBodySize()){
		response.clear();
		throw std::runtime_error(E400);
	}
	if (response.size())
		request.addToHeader("Content-Length", ftToString(response.size()));
}