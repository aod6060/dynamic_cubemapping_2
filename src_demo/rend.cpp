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
			std::cout << line << std::endl;
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
			std::cout << line << std::endl;
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
		glUniform1i(this->uniforms[name], x);
	}

	void Program::uniform2(std::string name, int x, int y) {
		glUniform2i(this->uniforms[name], x, y);
	}

	void Program::uniform3(std::string name, int x, int y, int z) {
		glUniform3i(this->uniforms[name], x, y, z);
	}

	void Program::uniform4(std::string name, int x, int y, int z, int w) {
		glUniform4i(this->uniforms[name], x, y, z, w);
	}

	// Floats
	void Program::uniform1(std::string name, float x) {
		glUniform1f(this->uniforms[name], x);
	}

	void Program::uniform2(std::string name, float x, float y) {
		glUniform2f(this->uniforms[name], x, y);
	}

	void Program::uniform3(std::string name, float x, float y, float z) {
		glUniform3f(this->uniforms[name], x, y, z);
	}

	void Program::uniform4(std::string name, float x, float y, float z, float w) {
		glUniform4f(this->uniforms[name], x, y, z, w);
	}

	// Matrices
	void Program::uniformMat2(std::string name, const glm::mat2& m) {
		glUniformMatrix2fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
	}

	void Program::uniformMat3(std::string name, const glm::mat3& m) {
		glUniformMatrix3fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
	}

	void Program::uniformMat4(std::string name, const glm::mat4& m) {
		glUniformMatrix4fv(this->uniforms[name], 1, GL_FALSE, &m[0][0]);
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
	void Program::createUniformBlock(std::string name, uint32_t binding) {
		ubos[name] = glGetUniformBlockIndex(this->program, name.c_str());
		glUniformBlockBinding(this->program, this->ubos[name], binding);
	}



	// Vertex Buffer
	void VertexBuffer::add1(float x) {
		this->lists.push_back(x);
	}

	void VertexBuffer::add2(float x, float y) {
		this->lists.push_back(x);
		this->lists.push_back(y);
	}

	void VertexBuffer::add3(float x, float y, float z) {
		this->lists.push_back(x);
		this->lists.push_back(y);
		this->lists.push_back(z);
	}

	void VertexBuffer::add4(float x, float y, float z, float w) {
		this->lists.push_back(x);
		this->lists.push_back(y);
		this->lists.push_back(z);
		this->lists.push_back(w);
	}
	
	void VertexBuffer::clear() {
		this->lists.clear();
	}

	void VertexBuffer::init() {
		glGenBuffers(1, &id);
	}

	void VertexBuffer::update() {
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, this->dataSize(), lists.data(), GL_DYNAMIC_DRAW);
		this->unbind();
	}

	void VertexBuffer::release() {
		glDeleteBuffers(1, &id);
		lists.clear();
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, this->id);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	uint32_t VertexBuffer::count() {
		return lists.size();
	}

	uint32_t VertexBuffer::dataSize() {
		return this->count() * sizeof(float);
	}

	uint32_t VertexBuffer::size1() {
		return sizeof(float);
	}

	uint32_t VertexBuffer::size2() {
		return sizeof(float) * 2;
	}

	uint32_t VertexBuffer::size3() {
		return sizeof(float) * 3;
	}

	uint32_t VertexBuffer::size4() {
		return sizeof(float) * 4;
	}

	// Index Buffer
	void IndexBuffer::add1(uint32_t x) {
		this->list.push_back(x);
	}

	void IndexBuffer::add2(uint32_t x, uint32_t y) {
		this->list.push_back(x);
		this->list.push_back(y);
	}

	void IndexBuffer::add3(uint32_t x, uint32_t y, uint32_t z) {
		this->list.push_back(x);
		this->list.push_back(y);
		this->list.push_back(z);
	}

	void IndexBuffer::add4(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {
		this->list.push_back(x);
		this->list.push_back(y);
		this->list.push_back(z);
		this->list.push_back(w);
	}

	void IndexBuffer::clear() {
		this->list.clear();
	}

	void IndexBuffer::init() {
		glGenBuffers(1, &id);
	}

	void IndexBuffer::update() {
		this->bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->dataSize(), this->list.data(), GL_STATIC_DRAW);
		this->unbind();
	}

	void IndexBuffer::release() {
		glDeleteBuffers(1, &id);
		this->list.clear();
	}

	void IndexBuffer::bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}

	void IndexBuffer::unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t IndexBuffer::count() {
		return list.size();
	}

	uint32_t IndexBuffer::dataSize() {
		return list.size() * sizeof(uint32_t);
	}

	uint32_t IndexBuffer::size1() {
		return sizeof(uint32_t);
	}

	uint32_t IndexBuffer::size2() {
		return sizeof(uint32_t) * 2;
	}

	uint32_t IndexBuffer::size3() {
		return sizeof(uint32_t) * 3;
	}

	uint32_t IndexBuffer::size4() {
		return sizeof(uint32_t) * 4;
	}

	// Uniform Buffers
	void UniformBuffer::init() {
		glGenBuffers(1, &id);
	}

	void UniformBuffer::release() {
		glDeleteBuffers(1, &id);
	}


	void UniformBuffer::update(void* data, uint32_t typeSize) {
		glBindBuffer(GL_UNIFORM_BUFFER, this->id);
		glBufferData(GL_UNIFORM_BUFFER, typeSize, data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void UniformBuffer::bind(uint32_t index, uint32_t typeSize) {
		glBindBufferRange(GL_UNIFORM_BUFFER, index, this->id, 0, typeSize);
	}





	// Texture2D
	void Texture2D::init() {
		glGenTextures(1, &this->id);
	}

	void Texture2D::release() {
		glDeleteTextures(1, &this->id);
	}

	void Texture2D::bind(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, this->id);
	}

	void Texture2D::unbind(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture2D::parameteri(GLenum type, int32_t value) {
		glTexParameteri(GL_TEXTURE_2D, type, value);
	}

	void Texture2D::genMipmaps() {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	void Texture2D::update(image::Image& image) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(image.bbp == 4) ? GL_RGBA : GL_RGB,
			image.width,
			image.height,
			0,
			(image.bbp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			image.pixels.data()
		);
	}

	void Texture2D::update(uint32_t width, uint32_t height, uint32_t bbp) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			(bbp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bbp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			nullptr);
	}

	void Texture2D::update(uint32_t width, uint32_t height, GLenum format, GLenum type) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			format,
			width,
			height,
			0,
			format,
			type,
			nullptr);
	}

	void createTexture2D(
		Texture2D& out,
		std::string path,
		uint32_t minFilter,
		uint32_t magFilter,
		bool generateMipmaps) {

		image::Image t;

		t.init(path);

		out.init();

		out.bind(GL_TEXTURE0);

		out.update(t);

		out.parameteri(GL_TEXTURE_MAG_FILTER, magFilter);
		out.parameteri(GL_TEXTURE_MIN_FILTER, minFilter);

		if (generateMipmaps) {
			out.genMipmaps();
		}

		out.unbind(GL_TEXTURE0);
		t.release();
	}



	// Cubemap
	uint32_t Cubemap::SIZE = 6;

	void Cubemap::init() {
		glGenTextures(1, &this->id);
	}
	
	void Cubemap::release() {
		glDeleteTextures(1, &this->id);
	}

	void Cubemap::bind(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
	}
	
	void Cubemap::unbind(GLenum unit) {
		glActiveTexture(unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void Cubemap::parameteri(GLenum type, int32_t value) {
		glTexParameteri(GL_TEXTURE_CUBE_MAP, type, value);
	}

	uint32_t getCubemap(uint32_t v) {
		return GL_TEXTURE_CUBE_MAP_POSITIVE_X + v;
	}

	// east
	void Cubemap::update(
		uint32_t index, 
		GLenum format, 
		GLenum type, 
		uint32_t width, 
		uint32_t height) {

		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,
			0,
			format,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			0);

	}

	void Cubemap::updateEast(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(0),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateEast(image::Image& image) {
		this->updateEast(image.width, image.height, image.bbp, image.pixels.data());
	}

	// west
	void Cubemap::updateWest(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(1),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateWest(image::Image& image) {
		this->updateWest(image.width, image.height, image.bbp, image.pixels.data());
	}

	// up
	void Cubemap::updateUp(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(2),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateUp(image::Image& image) {
		this->updateUp(image.width, image.height, image.bbp, image.pixels.data());
	}

	// down
	void Cubemap::updateDown(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(3),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateDown(image::Image& image) {
		this->updateDown(image.width, image.height, image.bbp, image.pixels.data());
	}

	// north
	void Cubemap::updateNorth(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(4),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateNorth(image::Image& image) {
		this->updateNorth(image.width, image.height, image.bbp, image.pixels.data());
	}

	// south
	void Cubemap::updateSouth(uint32_t width, uint32_t height, uint32_t bpp, void* pixel) {
		glTexImage2D(
			getCubemap(5),
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			width,
			height,
			0,
			(bpp == 4) ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			pixel);
	}
	
	void Cubemap::updateSouth(image::Image& image) {
		this->updateSouth(image.width, image.height, image.bbp, image.pixels.data());
	}

	void createCubemap(
		Cubemap& cubemap,
		std::string east,
		std::string west,
		std::string up,
		std::string down,
		std::string north,
		std::string south) {

		image::Image e, w, u, d, n, s;

		e.init(east);
		w.init(west);
		u.init(up);
		d.init(down);
		n.init(north);
		s.init(south);


		cubemap.init();

		cubemap.bind(GL_TEXTURE0);

		cubemap.updateEast(e);
		cubemap.updateWest(w);
		cubemap.updateUp(u);
		cubemap.updateDown(d);
		cubemap.updateNorth(n);
		cubemap.updateSouth(s);

		cubemap.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cubemap.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		cubemap.parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		cubemap.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		cubemap.parameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		cubemap.unbind(GL_TEXTURE0);

		e.release();
		w.release();
		u.release();
		d.release();
		n.release();
		s.release();
	}

	void createEmptyCubemap(
		Cubemap& cubemap, 
		uint32_t width, 
		uint32_t height) {

		cubemap.init();

		cubemap.bind(GL_TEXTURE0);

		cubemap.updateEast(width, height, 3, nullptr);
		cubemap.updateWest(width, height, 3, nullptr);
		cubemap.updateUp(width, height, 3, nullptr);
		cubemap.updateDown(width, height, 3, nullptr);
		cubemap.updateNorth(width, height, 3, nullptr);
		cubemap.updateSouth(width, height, 3, nullptr);

		cubemap.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cubemap.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		cubemap.parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		cubemap.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		cubemap.parameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		cubemap.unbind(GL_TEXTURE0);

	}



	// Renderbuffer
	void Renderbuffer::init() {
		glGenRenderbuffers(1, &this->id);
	}
	
	void Renderbuffer::release() {
		glDeleteRenderbuffers(1, &this->id);
	}

	void Renderbuffer::bind() {
		glBindRenderbuffer(GL_RENDERBUFFER, id);
	}

	void Renderbuffer::unbind() {
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	void Renderbuffer::update(GLenum format, uint32_t width, uint32_t height) {
		glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	}

	// Framebuffer
	void Framebuffer::init() {
		glGenFramebuffers(1, &this->id);
	}

	void Framebuffer::release() {
		glDeleteFramebuffers(1, &this->id);
	}

	void Framebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, this->id);
	}

	void Framebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::attachDepthBuffer(Texture2D& tex) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex.id, 0);
	}

	void Framebuffer::attachDepthBuffer(Renderbuffer& rend) {
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rend.id);
	}

	// color buffer
	void Framebuffer::attachColorBuffer(GLenum colorAttachment, Texture2D& tex) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, GL_TEXTURE_2D, tex.id, 0);
	}
	
	void Framebuffer::attachColorBuffer(GLenum colorAttachment, GLenum target, Cubemap& cubemap) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, colorAttachment, target, cubemap.id, 0);
	}


	void Framebuffer::drawBuffers(GLenum d) {
		glDrawBuffer(d);
	}

	void Framebuffer::readBuffer(GLenum d) {
		glReadBuffer(d);
	}

}

