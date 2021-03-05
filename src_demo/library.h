#pragma once


#include <sys.h>

namespace ftw {
	void init(ft::Table* table);
	ft::Table* get();
	void release();
}

namespace util {
	void readFile(std::string path, std::function<void(std::string)> cb);
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
		void createUniformBlock(std::string name, uint32_t location);
	};

}