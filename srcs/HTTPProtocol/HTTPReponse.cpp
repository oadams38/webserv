
#include "HTTPReponse.hpp"

// Default constructor
HTTPReponse::HTTPReponse() { return; }
HTTPReponse::HTTPReponse(std::string errorMsg, ServerConfig& conf) {
	int code = atoi(errorMsg.substr(0, errorMsg.find(':')).c_str());

	_statusLine = "HTTP/1.1 " + errorMsg.substr(0, errorMsg.find(':')) + "\r\n";

	_header =	headerLineFormat("Date", getTimeStamp()) + \
				headerLineFormat("Content-Type", "text/html");
	if (conf[code]){
		appendToVector(_body, fileToStr(conf.getErrorPage(code)));}
	else{
		appendToVector(_body, fileToStr("./error/400.html"));}
	_header += headerLineFormat("Content-Length", ftToString(_body.size()));
	formResponse();

	if (errorMsg.find(':') < errorMsg.size()){
		std::string message = errorMsg.substr(errorMsg.find(':')); // check if size tot needed;
		updateHTML(_body, message);
	}
}
HTTPReponse::HTTPReponse(std::vector<unsigned char> body, HTTPRequest& request) {
	std::string tmp;
	std::string statusStr;
	
	if (request.getStatus() == "200"){
		statusStr = " OK\r\n";
		_header =	headerLineFormat("Date", getTimeStamp()) \
				+headerLineFormat("Cache-Control", "no-cache");
		_body = body;
	}
	else if (request.getStatus() == "302"){
		statusStr = " Found\r\n";
		_header += headerLineFormat("Location", request.getHeader()["Location"]);
		_header += headerLineFormat("Content-Length", "0");
		_header += headerLineFormat("Connection", "close");
	}
	_statusLine = "HTTP/1.1 " + request.getStatus() + statusStr;
	
	if (!body.empty() && request.getContent().find(request.getCGIExt()) > request.getContent().size()) { //check for body or if CGI request.getCGIExt()
		isBinaryFile(request.getContent(), tmp);
		if (vecToStr(body).find("<!DOCTYPE html>") < request.getBody().size())
			tmp = "text/html; charset=UTF-8";
		_header += headerLineFormat("Content-Type", tmp);
		_header += headerLineFormat("Content-Length", ftToString(_body.size()));
	}
	else if (request.getContent().find(request.getCGIExt()) <= request.getContent().size()) {
		_header += headerLineFormat("Content-Type", "text/html; charset=UTF-8");
		_header += headerLineFormat("Content-Length", ftToString(_body.size()));
	}

	formResponse();
}
// Copy constructor
HTTPReponse::HTTPReponse(const HTTPReponse &other) {
  *this = other;
  return;
}

// Copy assignment overload
HTTPReponse &HTTPReponse::operator=(const HTTPReponse &rhs) {
  (void)rhs;
  return *this;
}

// Default destructor
HTTPReponse::~HTTPReponse() { return; }

std::vector<unsigned char> const	HTTPReponse::getFinal() const {
	return _final;
}
void	HTTPReponse::formResponse() {
	appendToVector(_final, _statusLine);
	appendToVector(_final, _header);
	appendToVector(_final, "\r\n");
	if (!_body.empty()){
		appendToVector(_final,_body);}
}

std::string HTTPReponse::headerLineFormat(std::string val, std::string content){
	return val + ": " + content + "\r\n";
}

void HTTPReponse::updateHTML(std::vector<unsigned char> body, std::string errorMsg){
	for (int i = 0; i < 26; i++)
		body.pop_back();
	
	appendToVector(body, "\t\t<p>");
	appendToVector(body, errorMsg);
	appendToVector(body, "</p>\r\n\t</div>\r\n</body>\r\n</html>\r\n");
}