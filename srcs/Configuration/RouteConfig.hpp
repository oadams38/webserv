/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RouteConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lvodak <lvodak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 17:41:23 by alexphil          #+#    #+#             */
/*   Updated: 2024/12/05 21:38:46 by lvodak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <string>
# include <stdexcept>

enum	Rules { GET, POST, DELETE, REDIRECT, ROOT, AUTOINDEX, INDEX, PATH, EXT, UPLOAD };

class	RouteConfig {

	public:
		RouteConfig();
		
		// Setters

		void			allowMethod(int method);
		void			setRedirect(std::string redirect);
		void			setRoot(std::string root);
		void			setAutoindex(bool state);
		void			setIndex(std::string index);
		void			setPath(std::string path);
		void			setExt(std::string ext);
		void			setUpload(std::string upload);

		// Getters

		bool			isMethodAllowed(int method);
		std::string		getRedirect();
		std::string 	getRoot();
		bool			hasAutoindex();
		std::string		getIndex();
		std::string		getPath();
		std::string		getExt();
		std::string		getUpload();
		
		// Operator
		
		bool			operator[](int RULE);
		
	private:
		bool			methods[3];
		std::string		redirect; //exclusive >> GET
		std::string		root; //non exclusif  >> GET
		bool			autoindex;
		std::string		index;
		std::string		path;
		std::string		ext;
		std::string		upload;
};
