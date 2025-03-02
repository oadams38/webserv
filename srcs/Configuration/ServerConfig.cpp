/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:16:46 by alexphil          #+#    #+#             */
/*   Updated: 2024/12/05 21:14:56 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerConfig.hpp"
# include "RouteConfig.hpp"

ServerConfig::ServerConfig() {
	host 				= "localhost";
	clientMaxBodySize	= 1024 * 1024; // 1MB
}

// SETTERS

void	ServerConfig::setHost(std::string name) {
	host = name;
}

void	ServerConfig::setPort(int port) {
	this->port = port;
}

std::map<std::string, RouteConfig> ServerConfig::getRoute(){
	return routes;
}

void	ServerConfig::addErrorPage(int code, std::string file) {
	errorPages[code] = file;
}

void	ServerConfig::setClientMaxBodySize(size_t size) {
	clientMaxBodySize = size;
}

void	ServerConfig::addRoute(std::string route, RouteConfig routeConfig) {
	routes[route] = routeConfig;
}

// GETTERS

std::string		ServerConfig::getHost() {
	return (host);
}

int				ServerConfig::getPort() {
	return (port);
}

std::string		ServerConfig::getErrorPage(int code) {
	return (errorPages[code]);
}

size_t			ServerConfig::getClientMaxBodySize() {
	return (clientMaxBodySize);
}

RouteConfig		&ServerConfig::getRoute(std::string route) {
	return (routes[route]);
}

// OVERLOADED OPERATOR

bool			ServerConfig::operator[](int code) {
	return (!getErrorPage(code).empty());
}

RouteConfig		&ServerConfig::operator[](std::string route) {
	return (getRoute(route));
}
