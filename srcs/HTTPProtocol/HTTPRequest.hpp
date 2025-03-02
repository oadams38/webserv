#ifndef HTTPREQUEST_HPP_
# define HTTPREQUEST_HPP_

# include <iostream>
# include "../../includes/webserv.hpp"
# include <fstream>
# include <sstream>

class ServerConfig;
class Client;

class HTTPRequest {
 public:
  HTTPRequest();
  HTTPRequest(Client *client, ServerConfig& conf);
  HTTPRequest(const HTTPRequest &other);
  HTTPRequest &operator=(const HTTPRequest &rhs);
  ~HTTPRequest();

  std::string					getMethod() const;
  std::string					getContent() const;
  std::string					getQuery() const;
  std::string					getStatus() const;
  std::string					getProtocolHTTP() const;
  std::string					getCGIPath() const;
  std::string					getCGIExt() const;
  std::string					getRoute() const;
  std::vector<unsigned char>&	getBody();
  std::map<std::string, std::string>	getHeader() const;

  void							addToHeader(std::string, std::string);
  void							setStatus(std::string);

 private:
  std::map<std::string, std::string> splitHeader(std::string request);
  void	divideUrlQuery(std::string statusLine);

  bool	incompleteHeader(std::vector<unsigned char> buffer);
  bool	incompleteBody(std::vector<unsigned char> buffer);

  bool	checkMethod(std::string method, ServerConfig& conf);
  void	checkLink(ServerConfig& conf);
  void	checkHeaders(ServerConfig& conf);
  
  void	setBody(std::vector<unsigned char> buffer);
  void	changePathURL(ServerConfig& conf);

  std::string							_method;
  std::string							_query;
  std::string							_content;
  std::string							_status;
  std::string							_protocolHTTP;
  std::vector<unsigned char>			_body;
  std::map<std::string, std::string>	_header;
  std::string							_CGIPath;
  std::string							_CGIExt;
  std::string							_route;
};

#endif // HTTPREQUEST_HPP_
