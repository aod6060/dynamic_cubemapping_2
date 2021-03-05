#include "library.h"


namespace util {
	
	void readFile(std::string path, std::function<void(std::string)> cb) {
		std::ifstream in(path);
		std::string temp;

		while (std::getline(in, temp)) {
			cb(temp);
		}

		in.close();
	}

}