#include "../../includes/webserv.hpp"

void	uploadFile(std::string boundary, HTTPRequest& request, std::vector<unsigned char>& response, std::vector<unsigned char>& body){
	std::string filename;
	std::string tmp;
	std::string bodyStr = vecToStr(body);
	std::stringstream s(vecToStr(body));

	getline(s, tmp, '\n');
	if (!boundary.empty() && tmp.find(boundary) < tmp.size()){
		getline(s, tmp, '\n');
		if (tmp.find("filename=") < tmp.size())
			filename = "/" + strTrim(tmp.substr(tmp.find("filename=") + 9), "\r\n\" ");
		for (int i = 0; i < 2; i++)
			getline(s, tmp, '\n');
	}
	if (filename.empty())
		throw std::runtime_error(E400);
	filename = (request.getContent() + filename);
	if (access(filename.c_str(), F_OK))
		request.setStatus("201");
	if (isBinaryFile(request.getContent(), tmp))
		StrToBinaryFile(filename, bodyStr, boundary);
	else
		StrToTextFile(filename, s, boundary, request);
	appendToVector(response, "File uploaded successfully.\r\n");
}

void	POSTsimpleContent(std::vector<unsigned char>& response, std::vector<unsigned char> body){
	appendToVector(response, "Input received and stocked: ");
	appendToVector(response, body);
}


void	executePOST(HTTPRequest& request, std::vector<unsigned char>& response, ServerConfig& conf) {
	std::vector<unsigned char> body = request.getBody();
	std::string Content = request.getHeader()["Content-Type"];
	std::string boundary;

	(void)conf;
	if (Content.find("multipart/form-data") < Content.size()) {
		boundary = Content.substr(Content.find("boundary=") + 9);
		boundary = strTrim(boundary, "\" \n");
		uploadFile(boundary, request, response, body);
	}
	else if (Content.find("application/x-www-form-urlencoded") < Content.size())
		throw std::runtime_error(E501 ": This POST option is not implemented");
	else if (Content.find("application/json") < Content.size())
		throw std::runtime_error(E501 ": This POST option is not implemented");
	else if (Content.find("text/plain") < Content.size())
		POSTsimpleContent(response, body);
	else{
		throw std::runtime_error(E400);}
}