#include "HTTPRequest.hpp"

// Default constructor
HTTPRequest::HTTPRequest() { return; }

HTTPRequest::HTTPRequest(Client *client, ServerConfig& conf) { 
	std::string request(vecToStr(client->getReadBuffer()));
	if (incompleteHeader(client->getReadBuffer()))
		throw std::runtime_error("incomplete");
	
	std::string tmp = request.substr(0, request.find_first_of('\r'));

	_method = tmp.substr(0, tmp.find_first_of(32));

	divideUrlQuery(tmp);

	try {
		checkLink(conf);}
	catch (std::exception& e){
		throw std::runtime_error(e.what());
	}

	if (!checkMethod(_method, conf))
		throw std::runtime_error(E405);

	_protocolHTTP = tmp.substr(tmp.find_last_of(' ') + 1, tmp.length() - tmp.find_last_of(' ')); // needed to check?
	if (_protocolHTTP.find("HTTP/1.1") > _protocolHTTP.size())
		throw std::runtime_error(E505);

	_header = splitHeader(request);
	if (incompleteBody(client->getReadBuffer())) {
		if (!_header["Content-Length"].empty())
			client->setBodySize(atoll(_header["Content-Length"].c_str()));
		throw std::runtime_error("incomplete");
	}
	client->setBodySize(-1);
	if (_header["Content-Length"].empty() == 0)
		setBody(client->getReadBuffer());
	else{
		_body.push_back('\0');}

	_status = "200";
	_route = "";
	_CGIPath = "/usr/bin/python3";
	_CGIExt = ".py";
	checkHeaders(conf);
	return;
}

// Copy constructor
HTTPRequest::HTTPRequest(const HTTPRequest &other) {
  *this = other;
  return;
}

// Copy assignment overload
HTTPRequest &HTTPRequest::operator=(const HTTPRequest &rhs) {
  (void)rhs;
  return *this;
}

// Default destructor
HTTPRequest::~HTTPRequest() {
	if (!_header.empty())
	_header.clear();
	return; }

std::string	HTTPRequest::getMethod() const {
	return _method;
}
std::string	HTTPRequest::getStatus() const {
	return _status;
}
std::string	HTTPRequest::getContent() const {
	return _content;
}
std::string	HTTPRequest::getQuery() const {
	return _query;
}
std::string	HTTPRequest::getCGIPath() const {
	return _CGIPath;
}
std::string	HTTPRequest::getCGIExt() const {
	return _CGIExt;
}
std::string	HTTPRequest::getRoute() const {
	return _route;
}
std::string	HTTPRequest::getProtocolHTTP() const {
	return _protocolHTTP;
}
std::vector<unsigned char>&	HTTPRequest::getBody() {
	return _body;
}
std::map<std::string, std::string>	HTTPRequest::getHeader() const {
	return _header;
}
void	HTTPRequest::setStatus(std::string status){
	_status = status;
}

std::map<std::string, std::string> HTTPRequest::splitHeader(std::string request) {
	std::string tmp;
	std::stringstream l(request);
	std::map<std::string, std::string> tab;

	getline(l, tmp, '\n');
	while (getline(l, tmp, '\n'))
	{
		if (tmp.empty() || tmp == "\r")
			break;
		if (tmp.find(':') <= tmp.size())
			tab[tmp.substr(0, tmp.find_first_of(':'))] = strTrim((std::string)tmp.substr(tmp.find_first_of(':') + 1, tmp.length() - tmp.find_first_of(':') - 1), "\r\t\n\v ");
	}
	return tab;
}

bool	HTTPRequest::checkMethod(std::string method, ServerConfig& conf) {
	std::string	route;
	if (method == "GET" && conf[_route][GET])
		return true;
	if (method == "POST" && conf[_route][POST])
		return true;
	if (method == "DELETE" && conf[_route][DELETE])
		return true;
	return false;
}
void	HTTPRequest::checkLink(ServerConfig& conf) {
	std::string path;

	changePathURL(conf);
	path = _content;
	if (path.find("..") < path.size())
		throw std::runtime_error("403 Forbidden: Unauthorized Path");
	if (!access(path.c_str(), F_OK) || path.find("http") < path.size() || path.find("autoindex") < path.size())
		return;
	path = _content; 
	throw std::runtime_error(E404 ": Ressource Not found");
}

void	HTTPRequest::changePathURL(ServerConfig& conf) {
	std::string urlBeg = _content;
	std::string final;
	std::string urlEnd = "";

	final = getRedirPath(conf, _method, urlBeg, urlEnd);
	if (urlBeg[0] == '/')
		_route = urlBeg;
	else
		_route = "/" + urlBeg;
	if (!urlBeg.size() || final.empty())
		return;
	if (_content.find(conf.getRoute()[_route].getExt())){
		_CGIExt = conf.getRoute()[urlBeg].getExt();
		_CGIPath = conf.getRoute()[urlBeg].getPath();
	}
	_content = final + urlEnd;
	if (_content[0] == '/')
		_content = "." + _content;
}

void	HTTPRequest::checkHeaders(ServerConfig& conf) {
	if (_header["Host"].empty())
		throw std::runtime_error(E400 ": Missing Host");

	if (_method == "POST"){
		if (_header["Content-Length"].empty())
			throw std::runtime_error(E411);
		if (_header["Content-Type"].empty() || (_header["Content-Length"].empty() && !_body.empty())
			|| !_header["Range"].empty() || !_header["If-Modified-Since"].empty() || !_header["If-None-Match"].empty())
			throw std::runtime_error(E400 ": Invalid POST Header");
	}

	if (_method == "DELETE")
		if ((_body.empty() && (!_header["Content-Type"].empty() || !_header["Content-Length"].empty())))
			throw std::runtime_error(E400 ": No Content Found");

	if (_body.size() > 2 && (_header["Content-Type"].empty() || _method == "GET"))
		throw std::runtime_error(E400 ": Body Not Allowed");
	if (_body.size() > 2 && _header["Content-Length"].empty())
		throw std::runtime_error(E400 ": Bad Header");

	if ((int)_body.size() - 1 > atoi(_header["Content-Length"].c_str())
		|| atoi(_header["Content-Length"].c_str()) > (int)conf.getClientMaxBodySize()
		|| _body.size() > conf.getClientMaxBodySize())
		throw std::runtime_error(E413);
}

void	HTTPRequest::addToHeader(std::string val, std::string cont) {
	_header[val] = cont;
}

bool	HTTPRequest::incompleteHeader(std::vector<unsigned char> buffer){
	std::string str = vecToStr(buffer);
	if (str.find("\r\n\r\n") < buffer.size())
		return false;
	return true;
}
bool	HTTPRequest::incompleteBody(std::vector<unsigned char> buffer){
	std::vector<unsigned char>::iterator it = buffer.begin();
	int l = 0;

	while (!findWithIter(it, buffer.end(), 4, "\r\n\r\n")){
		it++;
		l++;
	}
	for (int i = 0; i < 4; i++){
		it++;
		l++;
	}
	// std::cout<<"received == "<<buffer.size()<<"; total == "<<(size_t)atoll(getHeader()["Content-Length"].c_str()) + l<<std::endl;
	if (buffer.size() <(size_t)atoll(getHeader()["Content-Length"].c_str()) + l)
		return true;
	return false;
}

void	HTTPRequest::setBody(std::vector<unsigned char> buffer){
	std::vector<unsigned char>::iterator it = buffer.begin();

	while (!findWithIter(it, buffer.end(), 4, "\r\n\r\n"))
		it++;
	for (int i = 0; i < 4; i++)
		it++;
	while (it != buffer.end())
		_body.push_back(*it++);
}

void	HTTPRequest::divideUrlQuery(std::string statusLine){
	if (statusLine.find('?') >= statusLine.size()){
		_content = statusLine.substr(statusLine.find_first_of(' ') + 1, statusLine.find_last_of(' ') - statusLine.find_first_of(' ') - 1);
		_query = "";
	}
	else {
		_content = statusLine.substr(statusLine.find_first_of(' ') + 1, statusLine.find('?') - statusLine.find_first_of(' ') - 1);
		_query = statusLine.substr(statusLine.find('?') + 1, statusLine.find_last_of(' ') - statusLine.find('?') - 1);
	}
}
