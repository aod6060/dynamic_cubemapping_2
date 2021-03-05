#pragma once


#include <sys.h>

namespace ftw {
	void init(ft::Table* table);
	ft::Table* get();
	void release();
}

namespace util {
	void readFile(std::string path, std::function<void(std::string)> cb);
	void splitString(std::string line, char delim, std::function<void(std::string)> cb);

	glm::vec2 toVec2(std::string str);
	glm::vec3 toVec3(std::string str);
	glm::vec4 toVec4(std::string str);
}

namespace image {
	struct Image {
		uint32_t width;
		uint32_t height;
		uint32_t bbp;
		std::vector<char> pixels;

		void init(std::string path);
		void release();
	};
}

namespace rend {
	// Program

	struct Program {
		// Vertex Shader
		uint32_t vertexShader = 0;
		// Fragment Shader
		uint32_t fragShader = 0;
		// Program
		uint32_t program = 0;
		// Vertex Array
		uint32_t vertexArray = 0;

		std::map<std::string, uint32_t> uniforms;
		std::map<std::string, uint32_t> attributes;
		std::map<std::string, uint32_t> ubos;

		void init(std::string vsPath, std::string fsPath);
		void release();

		void bind();
		void unbind();

		virtual void onCreateUniformsAttributes() = 0;

		// Uniforms
		void createUniform(std::string name);
		
		// Integers
		void uniform1(std::string name, int x);
		void uniform2(std::string name, int x, int y);
		void uniform3(std::string name, int x, int y, int z);
		void uniform4(std::string name, int x, int y, int z, int w);
		
		// Floats
		void uniform1(std::string name, float x);
		void uniform2(std::string name, float x, float y);
		void uniform3(std::string name, float x, float y, float z);
		void uniform4(std::string name, float x, float y, float z, float w);

		// Matrices
		void uniformMat2(std::string name, const glm::mat2& m);
		void uniformMat3(std::string name, const glm::mat3& m);
		void uniformMat4(std::string name, const glm::mat4& m);

		// Attributes
		void createAttributes(std::string name, uint32_t value);
		void bindAttributes();
		void unbindAttribute();
		void enable(std::string name);
		void disable(std::string name);
		void ptr(std::string name, uint32_t size, GLenum type);

		// UniformBufferObjects
		void createUniformBlock(std::string name, uint32_t binding);
	};


	// Vertex Buffer
	struct VertexBuffer {
		std::vector<float> lists;
		uint32_t id;

		void add1(float x);
		void add2(float x, float y);
		void add3(float x, float y, float z);
		void add4(float x, float y, float z, float w);
		void clear();

		void init();
		void update();
		void release();

		void bind();
		void unbind();

		uint32_t count();
		uint32_t dataSize();

		static uint32_t size1();
		static uint32_t size2();
		static uint32_t size3();
		static uint32_t size4();
	};

	// Index Buffer
	struct IndexBuffer {
		std::vector<uint32_t> list;
		uint32_t id = 0;

		void add1(uint32_t x);
		void add2(uint32_t x, uint32_t y);
		void add3(uint32_t x, uint32_t y, uint32_t z);
		void add4(uint32_t x, uint32_t y, uint32_t z, uint32_t w);
		void clear();

		void init();
		void update();
		void release();

		void bind();
		void unbind();

		uint32_t count();
		uint32_t dataSize();

		static uint32_t size1();
		static uint32_t size2();
		static uint32_t size3();
		static uint32_t size4();
	};

	// Uniform Buffer
	struct UniformBuffer {
		uint32_t id = 0;

		void init();
		void release();

		void update(void* data, uint32_t typeSize);
		void bind(uint32_t id, uint32_t typeSize);
	};

	// Texture2D
	struct Texture2D {
		uint32_t id = 0;
		uint32_t width;
		uint32_t height;
		uint32_t bbp;

		void init();
		void release();

		void bind(GLenum unit);
		void unbind(GLenum unit);

		void parameteri(GLenum type, int32_t value);
		void genMipmaps();

		void update(image::Image& image);
		void update(uint32_t width, uint32_t height, uint32_t bbp);
	};
}


namespace mesh {

	struct Vertex {
		glm::vec3 vertex;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};

	struct Triangle {
		uint32_t v1;
		uint32_t v2;
		uint32_t v3;
	};

	struct Mesh {
		uint32_t version;
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
	};

	void init(std::string path, Mesh& mesh);
}