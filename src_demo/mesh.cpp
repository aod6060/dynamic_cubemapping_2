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


		// Tangents Computation
		mesh.tangents.resize(mesh.vertices.size());

		for (int i = 0; i < mesh.triangles.size(); i++) {
			Triangle tris = mesh.triangles[i];

			// Compute Edges
			glm::vec3 edge1 = mesh.vertices[tris.v2].vertex - mesh.vertices[tris.v1].vertex;
			glm::vec3 edge2 = mesh.vertices[tris.v3].vertex - mesh.vertices[tris.v1].vertex;

			glm::vec2 deltaUV1 = mesh.vertices[tris.v2].texCoord - mesh.vertices[tris.v1].texCoord;
			glm::vec2 deltaUV2 = mesh.vertices[tris.v3].texCoord - mesh.vertices[tris.v1].texCoord;

			// Compute f
			float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			// Compute Tangent
			mesh.tangents[tris.v1].tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			mesh.tangents[tris.v2].tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			mesh.tangents[tris.v3].tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

			// Compute BiTangent
			mesh.tangents[tris.v1].biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			mesh.tangents[tris.v2].biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			mesh.tangents[tris.v3].biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		}

	}


	// OpenGLMesh
	void OpenGLMesh::init(std::string path) {
		mesh::init(path, this->mesh);

		// Load up vertex buffers
		this->vertices.init();
		this->texCoords.init();
		this->normals.init();
		std::for_each(this->mesh.vertices.begin(), this->mesh.vertices.end(), [&](mesh::Vertex& v) {
			// Vertices
			this->vertices.add3(
				v.vertex.x,
				v.vertex.y,
				v.vertex.z);

			// TexCoords
			this->texCoords.add2(
				v.texCoord.x,
				v.texCoord.y);

			// Normals
			this->normals.add3(
				v.normal.x,
				v.normal.y,
				v.normal.z);

		});
		this->vertices.update();
		this->texCoords.update();
		this->normals.update();

		this->tangents.init();
		this->biTangents.init();
		std::for_each(mesh.tangents.begin(), mesh.tangents.end(), [&](Tangent& t) {
			this->tangents.add3(
				t.tangent.x,
				t.tangent.y,
				t.tangent.z
			);
			this->biTangents.add3(
				t.biTangent.x,
				t.biTangent.y,
				t.biTangent.z
			);
		});

		this->tangents.update();
		this->biTangents.update();

		// Load up index buffers
		this->index.init();
		std::for_each(mesh.triangles.begin(), mesh.triangles.end(), [&](mesh::Triangle& t) {
			index.add3(t.v1, t.v2, t.v3);
		});
		this->index.update();
	}

	void OpenGLMesh::release() {
		this->index.release();
		vertices.release();
		texCoords.release();
		normals.release();
		tangents.release();
		biTangents.release();
	}
}