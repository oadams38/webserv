/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oladams <oladams@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 13:38:01 by alexphil          #+#    #+#             */
/*   Updated: 2024/11/04 16:34:33 by oladams          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <string>
# include <set>
# include <map>
# include <vector>
# include <fstream>
# include <sstream>

# include "ServerConfig.hpp"
# include "RouteConfig.hpp"

class	Config {

	public:
		// Methods
		
		void		checkParameters(int ac, char** av);
		void		parseConfigFile();

		bool		hasExtension(std::string filename, std::string extension);
		bool		isDirective(std::string token);
		bool		isRule(std::string token);
		bool		indentLevel(size_t level);
		bool		isCommentLine();
		bool		isEmptyLine();
		
		void		processDirective(int port, std::vector <std::string> tokens);
		void		processRule(int port, std::string route, std::vector <std::string> tokens);	
		
		// Setters

		void		addPort(int port);
		void		addServer(int port, ServerConfig serverConfig);
		
		// Getters

		std::set	<int>					&getPorts();
		std::map 	<int, ServerConfig>		&getServers();
		std::vector <ServerConfig>			&getConfigs();
	
	private:
		std::ifstream						configFile;
		std::string							line;
		
		std::set 	<std::string>			directives;
		std::set 	<std::string>			rules;
		
		std::set	<int>					usedPorts;
		std::map	<int, ServerConfig>		serverConfigs;
		std::vector	<ServerConfig>			configs;
};
