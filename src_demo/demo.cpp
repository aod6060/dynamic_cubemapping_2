#include "library.h"

struct Matrices {
	glm::mat4 proj;
	glm::mat4 view;
};

struct MainProgram : public rend::Program {


	virtual void onCreateUniformsAttributes() {
		// Uniforms
		this->createUniform("model");
		this->createUniform("tex0");
		this->uniform1("tex0", 0);

		// Uniforms Buffer Object
		this->createUniformBlock("Matrices", 0);
		// Attributes
		this->createAttributes("vertices", 0);
		this->createAttributes("texCoords", 1);

		this->bindAttributes();
		this->enable("vertices");
		this->enable("texCoords");
		this->unbindAttribute();
	}

};

static Matrices matrices;
static MainProgram mainProg;
static rend::UniformBuffer matricesBuffer;
static rend::VertexBuffer verticesBuffer;
static rend::VertexBuffer texCoordsBuffer;

static rend::IndexBuffer indexBuffer;

static image::Image dirtImage;
static rend::Texture2D dirtTex2D;

static float yrot = 0.0f;

void demo_init(ft::Table* table) {
	ftw::init(table);

	glEnable(GL_DEPTH_TEST);

	mainProg.init("data/shaders/main.vs.glsl", "data/shaders/main.fs.glsl");

	// Uniform Buffer
	matricesBuffer.init();
	matricesBuffer.update(&matrices, sizeof(matrices));
	matricesBuffer.bind(0, sizeof(matrices));

	// Vertices Buffer
	verticesBuffer.init();
	verticesBuffer.add3(-0.5f, 0.5f, 0.0f);
	verticesBuffer.add3(0.5f, 0.5f, 0.0f);
	verticesBuffer.add3(-0.5f, -0.5f, 0.0f);
	verticesBuffer.add3(0.5f, -0.5f, 0.0f);
	verticesBuffer.update();

	// TexCoords Buffer
	texCoordsBuffer.init();
	texCoordsBuffer.add2(0.0f, 0.0f);
	texCoordsBuffer.add2(1.0f, 0.0f);
	texCoordsBuffer.add2(0.0f, 1.0f);
	texCoordsBuffer.add2(1.0f, 1.0f);
	texCoordsBuffer.update();

	// Index Buffer
	indexBuffer.init();
	indexBuffer.add3(0, 1, 2);
	indexBuffer.add3(2, 1, 3);
	indexBuffer.update();

	dirtImage.init("data/textures/water.png");

	dirtTex2D.init();
	dirtTex2D.bind(GL_TEXTURE0);

	dirtTex2D.update(dirtImage);
	dirtTex2D.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	dirtTex2D.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	dirtTex2D.genMipmaps();

	dirtTex2D.unbind(GL_TEXTURE0);
}

void demo_update(float delta) {

	yrot += 32.0f * delta;

	if (yrot > 360.0f) {
		yrot -= 360.0f;
	}
}

void demo_render() {

	matrices.proj =
		glm::perspective(glm::radians(45.0f), (float)ftw::get()->app.getWidth() / (float)ftw::get()->app.getHeight(), 1.0f, 1024.0f);


	matrices.view = glm::mat4(1.0f);

	matricesBuffer.update(&matrices, sizeof(matrices));

	glViewport(0, 0, ftw::get()->app.getWidth(), ftw::get()->app.getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	mainProg.bind();

	glm::mat4 model = 
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
		glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(0.0f, 1.0f, 0.0f));

	mainProg.uniformMat4("model", model);


	dirtTex2D.bind(GL_TEXTURE0);

	mainProg.bindAttributes();

	verticesBuffer.bind();
	mainProg.ptr("vertices", 3, GL_FLOAT);
	texCoordsBuffer.bind();
	mainProg.ptr("texCoords", 2, GL_FLOAT);
	verticesBuffer.unbind();

	//glDrawArrays(GL_TRIANGLES, 0, verticesBuffer.count());

	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.count(), GL_UNSIGNED_INT, nullptr);
	indexBuffer.unbind();

	mainProg.unbindAttribute();

	dirtTex2D.unbind(GL_TEXTURE0);

	mainProg.unbind();
}

void demo_release() {
	dirtTex2D.release();
	dirtImage.release();

	indexBuffer.release();

	texCoordsBuffer.release();
	verticesBuffer.release();
	matricesBuffer.release();
	mainProg.release();
	ftw::release();
}