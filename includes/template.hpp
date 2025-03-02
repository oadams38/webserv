#pragma once

#include "../../includes/webserv.hpp"
#include <sstream>

template <typename T>
std::string ftToString(T value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template <typename T>
bool	findWithIter(T& pos, T end, int size, std::string toFind) {
	int i = 0;
	for (T it = pos; it != end && i < size; it++) {
		if (*it != toFind[i++])
			return false;
	}
	return true;
}
