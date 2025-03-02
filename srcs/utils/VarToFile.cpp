#include "../../includes/webserv.hpp"

void	StrToBinaryFile(std::string filename, std::string& body, std::string boundary) {
	std::string::iterator it1 = body.begin();
	std::vector<char>str;

	if (!boundary.empty())
		for (size_t i = 0; i < body.find_first_of(boundary) + boundary.size(); i++){
			it1++;
		}
	for (std::string::iterator it = it1; it != body.end(); it++) {
		if (findWithIter(it, body.end(), boundary.size(), boundary))
			break;
		if (*it != '\r')
			str.push_back(*it);
	}
	std::ofstream file(filename.c_str(), std::ios::out);
	if (!file.is_open())
		throw std::runtime_error(E400);
	if (file.write(str.data(), str.size()).fail())
		std::cerr << "ofstream write() error" << std::endl;
	file.close();
}

void	StrToTextFile(std::string filename, std::stringstream& s, std::string boundary, HTTPRequest& request) {
	std::string tmp;
	std::ofstream file(filename.c_str(), std::ios::out);
	bool firstline = true;
	(void)request;
	
	if (!file.is_open())
		throw std::runtime_error(E400);
	while(getline(s, tmp)){
		if (!boundary.empty() && tmp.find(boundary) < tmp.size())
			break;
		if (!firstline)
			file<<std::endl;
		file<<strTrim(tmp, "\r\n");
		firstline = false;
	}
	file.close();
}