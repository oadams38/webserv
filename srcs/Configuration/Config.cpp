/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:37:56 by alexphil          #+#    #+#             */
/*   Updated: 2024/12/05 17:22:14 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include <cstring>

# include "Config.hpp"
# include "ServerConfig.hpp"

void	Config::checkParameters(int ac, char **av) {
	
	if (ac > 2)
		throw std::invalid_argument("Too many parameters");
	else if (ac == 2) {
		if (hasExtension(av[1], ".conf")) {
			configFile.open(av[1]);
			if (!configFile.is_open())
				throw std::runtime_error(std::string("Failed to open ") + av[1]);
		}
		else
			throw std::invalid_argument("Config file must be of *.conf type");
	}
	else {
		configFile.open("webserv.conf");
		if (!configFile.is_open())
			throw std::runtime_error("Failed to open default file");	
	}
}

void	Config::parseConfigFile() {
	
	directives.insert("listen");
	directives.insert("server_name");
	directives.insert("error_page");
	directives.insert("client_max_body_size");
	
	rules.insert("allow");
	rules.insert("redirect");
	rules.insert("root");
	rules.insert("autoindex");
	rules.insert("index");
	rules.insert("path");
	rules.insert("ext");
	rules.insert("upload");
	
	bool		inSrv	= false;
	bool		hasPort	= false;
	int			port	= -1;
	bool		inRoute = false;
	std::string	route	= "";

	while (std::getline(configFile, line)) {

		if (isEmptyLine() || isCommentLine())
			continue;
		
		std::istringstream			stream(line);
		std::string					token;
		std::vector <std::string>	tokens;
		
		while (stream >> token && token[0] != '#')
			tokens.push_back(token);
			
		if (indentLevel(0) && tokens[0] == "server") {
			if (tokens.size() != 1)
				throw std::invalid_argument("Server bloc shouldn't have any argument");
			if (inSrv && !hasPort)
				throw std::invalid_argument("No port specfied");
			inSrv	= true;
			hasPort = false;
			inRoute = false;
			continue;
		}
		else if (indentLevel(1) && inSrv && !inRoute && tokens[0] == "listen" && tokens.size() == 2) {
			if (hasPort)
				throw std::invalid_argument("Trying to define multiple ports for the same server");
			std::istringstream	stream(tokens[1]);
			if (!(stream >> port) || port < 1024 || port > 65535)
				throw std::invalid_argument("Port " + tokens[1] + " is outside expected ports range (1024 to 65535)");
			addPort(port);
			serverConfigs[port].setPort(port);
			hasPort = true;
			continue;
		}
		else if (indentLevel(1) && inSrv && !inRoute && isDirective(tokens[0]) && port > 0) {
			processDirective(port, tokens);
			continue;
		}
		else if (indentLevel(1) && inSrv && tokens[0] == "location" && tokens.size() == 2 && port > 0) {
			inRoute = true;
			route = tokens[1];
			continue;
		}
		else if (indentLevel(2) && inSrv && inRoute && isRule(tokens[0]) && port > 0) {
			processRule(port, route, tokens);
			continue;
		}
		else
			throw std::invalid_argument(std::string("Syntax error from " + tokens[0]));
	}
	
	if (!inSrv && configFile.peek() == EOF)
		throw std::runtime_error("No server defined");
	if (inSrv && !hasPort)
		throw std::invalid_argument("No port specfied");

	for (std::set<int>::iterator it = usedPorts.begin(); it != usedPorts.end(); it++)
		configs.push_back(serverConfigs[*it]);
}

void	Config::processDirective(int port, std::vector <std::string> tokens) {
	
	std::string			directive = tokens[0];
	
	if ((directive != "error_page" && tokens.size() != 2) 
		|| (directive == "error_page" && tokens.size() != 3))
		throw std::invalid_argument(std::string("Wrong number of args for ") + directive);
	
	std::istringstream	stream(tokens[1]);

	if (directive == "server_name")
		serverConfigs[port].setHost(tokens[1]);
	else if (directive == "error_page") {
		int	code;
		if (stream >> code && stream.eof())
			serverConfigs[port].addErrorPage(code, tokens[2]);
		else
			throw std::invalid_argument(std::string("Bad argument used with ") + directive);
	}
	else if (directive == "client_max_body_size") {
		size_t		size;
		std::string	suffix;
		if (stream >> size >> suffix && suffix == "M" && stream.eof())
			serverConfigs[port].setClientMaxBodySize(1024 * 1024 * size);
		else
			throw std::invalid_argument(std::string("Bad argument used with ") + directive);
	}
}

void	Config::processRule(int port, std::string route, std::vector <std::string> tokens) {
	std::string	rule = tokens[0];
	
	if (tokens.size() != 2)
		throw std::invalid_argument(std::string("Wrong number of args for ") + rule);

	if (rule == "allow") {
		if (tokens[1] == "GET")
			serverConfigs[port][route].allowMethod(GET);
		else if (tokens[1] == "POST")
			serverConfigs[port][route].allowMethod(POST);
		else if (tokens[1] == "DELETE")
			serverConfigs[port][route].allowMethod(DELETE);
		else
			throw std::invalid_argument(std::string("Trying to allow unknown method " + tokens[1]));
	}
	else if (rule == "redirect")
		serverConfigs[port][route].setRedirect(tokens[1]);
	else if (rule == "root")
		serverConfigs[port][route].setRoot(tokens[1]);
	else if (rule == "autoindex") {
		if (tokens[1] == "on")
			serverConfigs[port][route].setAutoindex(true);
		else if (tokens[1] == "off")
			serverConfigs[port][route].setAutoindex(false);
		else
			throw std::invalid_argument(std::string("Bad argument for ") + rule);
	}
	else if (rule == "index")
		serverConfigs[port][route].setIndex(tokens[1]);
	else if (rule == "path")
		serverConfigs[port][route].setPath(tokens[1]);
	else if (rule == "ext")
		serverConfigs[port][route].setExt(tokens[1]);
	else if (rule == "upload")
		serverConfigs[port][route].setUpload(tokens[1]);
}

bool	Config::hasExtension(std::string filename, std::string extension) {
	if (filename.length() <= extension.length())
		return (false);
	return (filename.substr(filename.length() - extension.length()) == extension);
}

bool	Config::isDirective(std::string token) {
	return (directives.find(token) != directives.end());
}

bool	Config::isRule(std::string token) {
	return (rules.find(token) != rules.end());
}

bool	Config::indentLevel(size_t level) {
	size_t	indent = 0;
	while (line[indent] == '\t')
		++indent;
	return (indent == level);
}

bool	Config::isEmptyLine() {
	for (size_t it = 0; it < line.length(); it++)
		if (!isspace(line[it]))
			return (false);
	return (true);
}

bool	Config::isCommentLine() {
	size_t	it = 0;
	while (it < line.length() && isspace(line[it]))
		++it;
	return (it < line.length() && line[it] == '#');
}

// SETTERS

void							Config::addPort(int port) {
	if (usedPorts.find(port) != usedPorts.end()) {
		std::ostringstream oss;
		oss << "Port " << port << " is already defined for "
		<< serverConfigs[port].getHost() << ":" << port;
		throw std::invalid_argument(oss.str());
	}
	usedPorts.insert(port);
}

void							Config::addServer(int port, ServerConfig srv) {
	serverConfigs[port] = srv;
}

// GETTERS

std::set <int>					&Config::getPorts() {
	return (usedPorts);
}

std::map <int, ServerConfig>	&Config::getServers() {
	return (serverConfigs);
}

std::vector <ServerConfig>		&Config::getConfigs() {
	return (configs);
}
