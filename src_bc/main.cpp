#include <iostream>
#include <fstream>
#include <sstream>

std::string path = "E:\\demo\\dynamic_cubemapping_2\\bin\\";

int main(int argc, char** argv) {
	uint64_t build = 0;

	std::stringstream ss;

	ss << path << argv[1];

	std::ifstream in(ss.str(), std::ios::binary);

	if (in.good()) {
		in.read((char*)&build, sizeof(uint64_t));
		in.close();
	}

	build += 1;

	std::cout << "Build: " << build << std::endl;

	std::ofstream out(ss.str(), std::ios::binary);

	out.write((char*)&build, sizeof(uint64_t));
	out.close();

	return 0;
}