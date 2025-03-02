#include "../../includes/webserv.hpp"
#include <ctime>
#include <sys/stat.h>

std::string strTrim(std::string str, std::string set) {
	int	n;
	int	start;

	if (str.empty())
		return ("");
	start = 0;
	while (set.find(str[start], 0) <= str.size())
		start++;
	n = str.size() - 1;
	while (set.find(str[n], 0) <= str.size() && n != 0)
		n--;
	return (str.substr(start, n - start + 1));
}

std::string	intToHex(int n) {
	std::stringstream ss;
	ss << std::hex << n;
	std::string res (ss.str());
	return res;
}

std::string	getTimeStamp() {
	time_t timestamp;
	time(&timestamp);

	std::string timeStr(ctime(&timestamp));
	return strTrim(timeStr, "\n");
}

std::string getRedirPath(ServerConfig& conf, std::string method, std::string& url, std::string& content){
	struct stat isDir;
	std::string final;

	stat((url + content).c_str(), &isDir);
	while (1){
		if (url.find("http") < url.size())
			return url;
		if (S_ISDIR(isDir.st_mode) && url != "/"){
			if (conf[url][INDEX] && content.empty())
				return conf[url].getIndex().substr(1); //may need to pass by another var to be sent
			if (conf[url][AUTOINDEX] && method == "GET" && content.empty())
				return final + "/autoindex";
			else 
				break;
		}
		else {
			if (conf[url][UPLOAD] && method == "GET")
				final = conf[url].getUpload();
			else if (conf[url][UPLOAD] && method != "GET")
				return conf[url].getUpload();
			if (conf[url][REDIRECT] && method == "GET"){
				final = conf[url].getRedirect();
				if (content.empty() && !access(final.substr(1).c_str(), F_OK))
					return final.substr(1);
			}
			if (conf[url][ROOT] && method != "DELETE")
				final = conf[url].getRoot();
			if (final.empty())
				final = url;
		}
		final = final.substr((final[0] == '/' && final.size()));
		stat(("./" + final + content).c_str(), &isDir);

		if (S_ISDIR(isDir.st_mode) && url != "/")
			continue;
		else if  (S_ISDIR(isDir.st_mode))
			break;
		else if (!access((final + content).c_str(), F_OK) || final.find("http") <=final.size())
			return final;
		else {
			if (url == "/")
				break;
			content = url.substr(url.find_last_of("/"));
			url = url.substr(0, url.find_last_of("/"));
			if (url.empty())
				url = "/";
		}
	}
	return "";
}


bool	isBinaryFile(std::string filename, std::string& type) {
	std::string image[] = {".png", ".jpg", ".jpeg", ".gif", ".bmp", ".tiff", ".ico", ".svg", ""};
	std::string audio[] = {".mp3", ".wav", ".flac", ".ogg", ".aac", ".wma", ""}; 
	std::string video[] = {".mp4", ".avi", ".mov", ".mkv", ".wmv", ".flv", ""}; 
	std::string	exec[] = {".exe", ".dll", ".so", ".bin", ".elf", ".dylib", ".out", ""};
	std::string	compr[] = {".zip", ".rar", ".tar", ".gz", ".7z", ".bz2", ""};
	std::string	doc[] = {".pdf", ".doc", ".docx", ".xls", ".xlsx", ".ppt", ".pptx", ""};
	std::string	oth[] = {".psd", ".iso", ".dat", ".class", ".swf", ".ttf", ".otf", ""};

	for (int i = 0; !image[i].empty(); i++){
		if (filename.find(image[i]) < filename.size()) {
			switch (i){
				case 6:
					type = "image/x-icon";
					break;
				case 7:
					type = "image/svg+xml";
					break;
				default:
					type = "image/" + image[i].substr(1);
			}
		return true;
	}}
	for (int i = 0; !audio[i].empty(); i++){
		if (filename.find(audio[i]) < filename.size()) {
			switch (i){
				case 6:
					type = "audio/x-ms-wma";
					break;
				default:
					type = "audio/" + audio[i].substr(1);
			}
		return true;
	}}
	for (int i = 0; !video[i].empty(); i++){
		if (filename.find(video[i]) < filename.size()) {
			switch (i){
				case 0:
					type = "video/mp4";
					break;
				case 1:
					type = "video/x-msvideo";
					break;
				case 2:
					type = "video/quicktime";
					break;
				case 3:
					type = "video/x-matroska";
					break;
				case 4:
					type = "video/x-ms-wmv";
					break;
				case 5:
					type = "video/x-flv";
					break;
			}
		return true;
	}}
	for (int i = 0; !exec[i].empty(); i++){
		if (filename.find(exec[i]) < filename.size()) {
			switch (i){
				case 0:
					type = "application/x-msdownload";
					break;
				case 1:
					type = "application/x-msdownload";
					break;
				default:
					type = "application/octet-stream";
					break;
			}
		return true;
	}}
	for (int i = 0; !compr[i].empty(); i++){
		if (filename.find(compr[i]) < filename.size()) {
			switch (i){
				case 0:
					type = "application/zip";
					break;
				case 1:
					type = "application/vnd.rar";
					break;
				case 2:
					type = "application/x-tar";
					break;
				case 3:
					type = "application/gzip";
					break;
				case 4:
					type = "application/x-7z-compressed";
					break;
				case 5:
					type = "application/x-bzip2";
					break;
			}
		return true;
	}}
	for (int i = 0; !doc[i].empty(); i++){
		if (filename.find(doc[i]) < filename.size()) {
			switch (i){
				case 0:
					type = "application/pdf";
					break;
				case 1:
					type = "application/msword";
					break;
				case 2:
					type = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
					break;
				case 3:
					type = "application/vnd.ms-excel";
					break;
				case 4:
					type = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
					break;
				case 5:
					type = "application/vnd.ms-powerpoint";
					break;
				case 6:
					type = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
					break;
			}
		return true;
	}}
	for (int i = 0; !oth[i].empty(); i++){
		if (filename.find(oth[i]) < filename.size()) {
			switch (i){
				case 0:
					type = "image/vnd.adobe.photoshop";
					break;
				case 1:
					type = "application/x-iso9660-image";
					break;
				case 2:
					type = "application/octet-stream";
					break;
				case 3:
					type = "application/java-vm";
					break;
				case 4:
					type = "application/x-shockwave-flash";
					break;
				case 5:
					type = "font/ttf";
					break;
				case 6:
					type = "font/otf";
					break;
			}
		return true;
	}}
	if (filename.find("html") < filename.size())
		type = "text/html; charset=UTF-8";
	else
		type = "text/plain";
	return false;
}

void	appendToVector(std::vector<unsigned char>& vec, std::vector<unsigned char> container){
	vec.insert(vec.end(), container.begin(), container.end());
}
void	appendToVector(std::vector<unsigned char>& vec, std::string container){
	vec.insert(vec.end(), container.begin(), container.end());
}

void callSelect(int maxFd, fd_set *readFds, fd_set *writeFds) {
	struct timeval			selectTimeout;
	selectTimeout.tv_sec = 0; selectTimeout.tv_usec = 50000;

	if (select(maxFd + 1, readFds, writeFds, NULL, &selectTimeout) < 0)
		throw std::exception();
}

void safeWrite(int fd, std::vector<unsigned char> writeVect, int n_tries) {
	if (n_tries == 0)
		throw std::runtime_error(E500);
	else if (writeVect.empty())
		return;
	fd_set writeFd;
	std::string tmp = vecToStr(writeVect); // macOs compiler was not working if not created as a temp string before 
	const char *writeBuffer = reinterpret_cast<const char *>(tmp.c_str());

	FD_ZERO(&writeFd);
	FD_SET(fd, &writeFd);
	callSelect(fd, NULL, &writeFd);
	if (!FD_ISSET(fd, &writeFd))
		throw std::runtime_error(E500);
	int ret = write(fd, writeBuffer, writeVect.size());
	if (ret < 0)
		throw std::runtime_error(E500);
	writeVect.erase(writeVect.begin(), writeVect.begin() + ret);
	safeWrite(fd, writeVect, --n_tries);
}

std::vector<unsigned char> safeRead(int fd) {
	fd_set readFd;
	unsigned char buffer[BUFFER_SIZE];
	

	FD_ZERO(&readFd);
	FD_SET(fd, &readFd);
	callSelect(fd, &readFd, NULL);
	if (!FD_ISSET(fd, &readFd)){
		throw std::runtime_error(E500);}
	int ret = read(fd, buffer, BUFFER_SIZE);
	if (ret < 0)
		throw std::runtime_error(E500);
	std::vector<unsigned char> readVect(buffer, buffer + ret);
	if (ret == BUFFER_SIZE) {
		std::vector<unsigned char> readMaterial = safeRead(fd);
		readVect.insert(readVect.end(), readMaterial.begin(), readMaterial.end());
	}
	return readVect;
}