/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:41:17 by alexphil          #+#    #+#             */
/*   Updated: 2024/12/05 21:13:53 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <string>
# include <map>

# include "RouteConfig.hpp"

class	ServerConfig {

	public:
		ServerConfig();

		// Setters

		void			setHost(std::string name);
		void			setPort(int port);
		void			addErrorPage(int code, std::string file);
		void			setClientMaxBodySize(size_t size);
		void			addRoute(std::string route, RouteConfig routeConfig);		
		
		// Getters

		std::string		getHost();
		int				getPort();
		std::string		getErrorPage(int code);
		size_t			getClientMaxBodySize();
		RouteConfig		&getRoute(std::string route);
		std::map <std::string, RouteConfig> getRoute();

		// Operators

		bool			operator[](int code);
		RouteConfig		&operator[](std::string route);
		
	private:
		int									port;
		std::string							host;
		std::map <int, std::string>			errorPages;
		size_t								clientMaxBodySize;
		std::map <std::string, RouteConfig>	routes;
};
