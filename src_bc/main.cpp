#include <iostream>
#include <fstream>

std::string path = "build.bin";

int main() {
	uint64_t build = 0;

	std::ifstream in(path, std::ios::binary);

	if (in.good()) {
		in.read((char*)&build, sizeof(uint64_t));
		in.close();
	}

	build += 1;

	std::cout << "Build: " << build << std::endl;

	std::ofstream out(path, std::ios::binary);

	out.write((char*)&build, sizeof(uint64_t));
	out.close();

	return 0;
}