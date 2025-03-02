/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:17:11 by alexphil          #+#    #+#             */
/*   Updated: 2024/12/05 21:38:50 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "RouteConfig.hpp"

RouteConfig::RouteConfig() {
	methods[GET] 	= true;
	methods[POST] 	= false;
	methods[DELETE] = false;
	redirect 		= "";
	root 			= "";
	autoindex 		= false;
	index 			= "";
	path			= "";
	ext				= "";
	upload 			= "";
}

// SETTERS

void	RouteConfig::allowMethod(int method) {
	methods[method] = true;
}

void	RouteConfig::setRedirect(std::string redirect) {
	this->redirect = redirect;
}

void	RouteConfig::setRoot(std::string root) {
	this->root = root;
}

void	RouteConfig::setAutoindex(bool state) {
	autoindex = state;
}

void	RouteConfig::setIndex(std::string index) {
	this->index = index;
}

void	RouteConfig::setPath(std::string path) {
	this->path = path;
}

void	RouteConfig::setExt(std::string ext) {
	this->ext = ext;
}

void	RouteConfig::setUpload(std::string upload) {
	this->upload = upload;
}

// GETTERS

bool			RouteConfig::isMethodAllowed(int method) {
	return (methods[method]);
}

std::string		RouteConfig::getRedirect() {
	return (redirect);
}

std::string 	RouteConfig::getRoot() {
	return (root);
}

bool			RouteConfig::hasAutoindex() {
	return (autoindex);
}

std::string		RouteConfig::getIndex() {
	return (index);
}

std::string		RouteConfig::getPath() {
	return (path);
}

std::string		RouteConfig::getExt() {
	return (ext);
}

std::string		RouteConfig::getUpload() {
	return (upload);
}

// Overloaded Operator

bool			RouteConfig::operator[](int RULE) {
	switch (RULE) {
		case GET:
		case POST:
		case DELETE:	return	(isMethodAllowed(RULE));
		case REDIRECT: 	return	(!getRedirect().empty());
		case ROOT: 		return	(!getRoot().empty());
		case AUTOINDEX: return	(hasAutoindex());
		case INDEX: 	return	(!getIndex().empty());
		case PATH:		return 	(!getPath().empty());
		case EXT:		return 	(!getExt().empty());
		case UPLOAD:	return	(!getUpload().empty());
		default:
			throw std::invalid_argument("Invalid rule sent");
	}
}
