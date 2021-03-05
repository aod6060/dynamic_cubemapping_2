#include "library.h"


namespace mesh {


	void init(std::string path, Mesh& mesh) {

		std::vector<std::string> contents;

		util::readFile(path, [&](std::string line) {
			contents.push_back(line);
		});


		for (int i = 0; i < contents.size(); i++) {
			std::vector<std::string> args;

			util::splitString(contents[i], ' ', [&](std::string arg) {
				args.push_back(arg);
			});


			if (args[0] == "version") {
				mesh.version = std::stoi(args[1]);
			}
			else if (args[0] == "vertex") {
				Vertex v;
				v.vertex = util::toVec3(args[1]);
				v.texCoord = util::toVec2(args[2]);
				v.normal = util::toVec3(args[3]);
				mesh.vertices.push_back(v);
			}
			else if (args[0] == "triangle") {
				Triangle t;
				t.v1 = std::stoi(args[1]);
				t.v2 = std::stoi(args[2]);
				t.v3 = std::stoi(args[3]);
				mesh.triangles.push_back(t);
			}
			args.clear();
		}
	}


}