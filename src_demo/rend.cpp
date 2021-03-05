#include "library.h"


namespace rend {
	// Program

	void Program::init(std::string vsPath, std::string fsPath) {
		// Shaders
		// Vertex Shader
		this->vertexShader = glCreateShader(GL_VERTEX_SHADER);
		
		std::stringstream ss;

		util::readFile(vsPath, [&](std::string line) {
			ss << line << std::endl;
		});

		std::string src = ss.str();
		const char* c_src = src.c_str();

		glShaderSource(this->vertexShader, 1, &c_src, nullptr);
		glCompileShader(this->vertexShader);

		int len;
		glGetShaderiv(this->vertexShader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			std::string log;
			log.resize(len);
			glGetShaderInfoLog(this->vertexShader, log.size(), nullptr, log.data());
			std::cout << log << std::endl;
		}

		// Fragment Shader
		this->fragShader = glCreateShader(GL_FRAGMENT_SHADER);


		ss.str("");
		util::readFile(fsPath, [&](std::string line) {
			ss << line << std::endl;
		});

		src = ss.str();
		c_src = src.c_str();

		glShaderSource(this->fragShader, 1, &c_src, nullptr);
		glCompileShader(this->fragShader);

		glGetShaderiv(this->fragShader, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			std::string log;
			log.resize(len);
			glGetShaderInfoLog(this->vertexShader, log.size(), nullptr, log.data());
			std::cout << log << std::endl;
		}

		// Program
		this->program = glCreateProgram();
		glAttachShader(this->program, this->vertexShader);
		glAttachShader(this->program, this->fragShader);
		glLinkProgram(this->program);

		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &len);
		if (len > 0) {
			std::string log;
			log.resize(len);
			glGetProgramInfoLog(this->vertexShader, log.size(), nullptr, log.data());
			std::cout << log << std::endl;
		}

		// Vertex Array
		glGenVertexArrays(1, &this->vertexArray);


		this->bind();
		this->onCreateUniformsAttributes();
		this->unbind();
	}

	void Program::release() {
		glDeleteVertexArrays(1, &this->vertexArray);

		glDetachShader(this->program, this->vertexShader);
		glDetachShader(this->program, this->fragShader);
		glDeleteProgram(this->program);

		glDeleteShader(this->vertexShader);
		glDeleteShader(this->fragShader);
	}

	void Program::bind() {
		glUseProgram(this->program);
	}

	void Program::unbind() {
		glUseProgram(0);
	}

	// Uniforms
	void Program::createUniform(std::string name) {
		this->uniforms[name] = glGetUniformLocation(this->program, name.c_str());
	}

	// Integers
	void Program::uniform1(std::string name, int x) {
	
	}

	void Program::uniform2(std::string name, int x, int y) {
	
	}

	void Program::uniform3(std::string name, int x, int y, int z) {
	
	}

	void Program::uniform4(std::string name, int x, int y, int z, int w) {
	
	}

	// Floats
	void Program::uniform1(std::string name, float x) {
	
	}

	void Program::uniform2(std::string name, float x, float y) {
	
	}

	void Program::uniform3(std::string name, float x, float y, float z) {
	
	}

	void Program::uniform4(std::string name, float x, float y, float z, float w) {
	
	}

	// Matrices
	void Program::uniformMat2(std::string name, const glm::mat2& m) {
	
	}

	void Program::uniformMat3(std::string name, const glm::mat3& m) {
	
	}

	void Program::uniformMat4(std::string name, const glm::mat4& m) {
	
	}


	// Attributes
	void Program::createAttributes(std::string name, uint32_t value) {
		this->attributes[name] = value;
	}

	void Program::bindAttributes() {
		glBindVertexArray(this->vertexArray);
	}

	void Program::unbindAttribute() {
		glBindVertexArray(0);
	}

	void Program::enable(std::string name) {
		glEnableVertexAttribArray(this->attributes[name]);
	}

	void Program::disable(std::string name) {
		glDisableVertexAttribArray(this->attributes[name]);
	}

	void Program::ptr(std::string name, uint32_t size, GLenum type) {
		glVertexAttribPointer(
			attributes[name], 
			size, 
			type, 
			GL_FALSE, 
			0, 
			nullptr);
	}

	// UniformBufferObjects
	void createUniformBlock(std::string name, uint32_t location) {

	}
}

