
#ifndef HTTPREPONSE_HPP_
# define HTTPREPONSE_HPP_

# include "HTTPRequest.hpp"
# include "../../includes/webserv.hpp"
# include <iostream>

class HTTPRequest;
class ServerConfig;

class HTTPReponse {
 public:
  HTTPReponse();
  HTTPReponse(std::string errorMsg, ServerConfig& conf);
  HTTPReponse(std::vector<unsigned char> body, HTTPRequest& request);
  HTTPReponse(std::string body, HTTPRequest& request);
  HTTPReponse(const HTTPReponse &other);
  HTTPReponse &operator=(const HTTPReponse &rhs);
  ~HTTPReponse();
  
  std::vector<unsigned char> const getFinal() const;
  void updateHTML(std::vector<unsigned char> body, std::string errorMsg);

 private:
  void	formResponse();
  std::string headerLineFormat(std::string val, std::string content);

  std::string					_statusLine;
  std::string					_header;
  std::vector<unsigned char>	_body;
  std::vector<unsigned char>	_final;
};

#endif // HTTPREPONSE_HPP_
