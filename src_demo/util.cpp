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

	void splitString(std::string line, char delim, std::function<void(std::string)> cb) {
		std::stringstream ss(line);
		std::string temp;

		while (std::getline(ss, temp, delim)) {
			cb(temp);
		}
	}

	glm::vec2 toVec2(std::string str) {
		std::vector<std::string> args;

		splitString(str, ':', [&](std::string arg) {
			args.push_back(arg);
		});

		return glm::vec2(
			std::stof(args[0]),
			std::stof(args[1])
		);
	}

	glm::vec3 toVec3(std::string str) {
		std::vector<std::string> args;

		splitString(str, ':', [&](std::string arg) {
			args.push_back(arg);
		});

		return glm::vec3(
			std::stof(args[0]),
			std::stof(args[1]),
			std::stof(args[2])
		);
	}
	
	glm::vec4 toVec4(std::string str) {
		std::vector<std::string> args;

		splitString(str, ':', [&](std::string arg) {
			args.push_back(arg);
		});

		return glm::vec4(
			std::stof(args[0]),
			std::stof(args[1]),
			std::stof(args[2]),
			std::stof(args[3])
		);
	}

}