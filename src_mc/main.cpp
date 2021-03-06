#include <library.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#define MESH_VERSION 1

void loadAssimpMesh(std::string path, mesh::Mesh& mesh);
void saveMesh(std::string path, mesh::Mesh& mesh);

int main(int argc, char** argv) {

	if (argc == 3) {
		mesh::Mesh mesh;
		std::string input = argv[1];
		std::string output = argv[2];

		std::cout << input << ": " << output << std::endl;

		loadAssimpMesh(input, mesh);
		saveMesh(output, mesh);
	}
	else {
		std::cout << "mc [input-path] [output-path]" << std::endl;
	}
	return 0;
}


void loadAssimpMesh(std::string path, mesh::Mesh& mh) {
	Assimp::Importer imp;

	const aiScene* scene = imp.ReadFile(
		path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs);

	aiMesh* m = scene->mMeshes[0];

	mh.version = MESH_VERSION;

	// Vertices
	for (int i = 0; i < m->mNumVertices; i++) {
		mesh::Vertex v;
		v.vertex = glm::vec3(
			m->mVertices[i].x,
			m->mVertices[i].z,
			m->mVertices[i].y
		);

		v.texCoord = glm::vec2(
			m->mTextureCoords[0][i].x,
			m->mTextureCoords[0][i].y
		);

		v.normal = glm::vec3(
			m->mNormals[i].x,
			m->mNormals[i].z,
			m->mNormals[i].y
		);

		
		mh.vertices.push_back(v);
	}

	// Triangles
	for (int i = 0; i < m->mNumFaces; i++) {
		mesh::Triangle t;
		t.v1 = m->mFaces[i].mIndices[0];
		t.v2 = m->mFaces[i].mIndices[2];
		t.v3 = m->mFaces[i].mIndices[1];
		mh.triangles.push_back(t);
	}
}

std::string toStr(glm::vec2 v) {
	std::stringstream ss;
	ss << v.x << ":" << v.y;
	return ss.str();
}

std::string toStr(glm::vec3 v) {
	std::stringstream ss;
	ss << v.x << ":" << v.y << ":" << v.z;
	return ss.str();
}

void saveMesh(std::string path, mesh::Mesh& mesh) {
	std::ofstream out(path);


	// Vertices
	out << "version " << mesh.version << std::endl;

	// Vertex
	std::for_each(mesh.vertices.begin(), mesh.vertices.end(), [&](mesh::Vertex& v) {
		out << "vertex ";
		out << toStr(v.vertex) << " ";
		out << toStr(v.texCoord) << " ";
		out << toStr(v.normal) << std::endl;
	});

	// Triangle
	std::for_each(mesh.triangles.begin(), mesh.triangles.end(), [&](mesh::Triangle& t) {
		out << "triangle ";
		out << t.v1 << " ";
		out << t.v2 << " ";
		out << t.v3 << std::endl;
	});

	out.close();
}
