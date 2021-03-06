#include "library.h"

struct Matrices {
	glm::mat4 proj;
	glm::mat4 view;
};

struct Light {
	glm::vec3 direction;
	float nu1;
	glm::vec3 ambient;
	float nu2;
	glm::vec3 diffuse;
	float nu3;
	glm::vec3 specular;
	float nu4;
};

struct Material {
	float power = 0.5f;
	glm::vec3 nu;
};

struct MainProgram : public rend::Program {


	virtual void onCreateUniformsAttributes() {
		// Uniforms
		this->createUniform("model");
		this->createUniform("normalMatrix");
		this->createUniform("cameraPos");

		this->createUniform("albedo");
		this->uniform1("albedo", 0);

		this->createUniform("normalMap");
		this->uniform1("normalMap", 1);

		this->createUniform("specularMap");
		this->uniform1("specularMap", 2);

		// Uniforms Buffer Object
		this->createUniformBlock("Matrices", 0);
		this->createUniformBlock("Light", 1);
		this->createUniformBlock("Material", 2);

		// Attributes
		this->createAttributes("vertices", 0);
		this->createAttributes("texCoords", 1);
		this->createAttributes("normals", 2);
		this->createAttributes("tangents", 3);
		this->createAttributes("biTangents", 4);

		this->bindAttributes();
		this->enable("vertices");
		this->enable("texCoords");
		this->enable("normals");
		this->enable("tangents");
		this->enable("biTangents");
		this->unbindAttribute();
	}

};

static Matrices matrices;
static Light light;
static Material material;

static MainProgram mainProg;

static rend::UniformBuffer matricesBuffer;
static rend::UniformBuffer lightBuffer;
static rend::UniformBuffer materialBuffer;

static mesh::OpenGLMesh floorMesh;
static mesh::OpenGLMesh cubeMesh;
static mesh::OpenGLMesh cylenderMesh;
static mesh::OpenGLMesh monkeyFaceMesh;
static mesh::OpenGLMesh sphereMesh;
static mesh::OpenGLMesh torusMesh;

/*
static rend::Texture2D grassTex2D;
static rend::Texture2D dirtTex2D;
static rend::Texture2D beachSandTex2D;
static rend::Texture2D seaFloorTex2D;
static rend::Texture2D brickTex2D;
static rend::Texture2D waterTex2D;
*/

struct TextureMaterial {
	rend::Texture2D albedo;
	rend::Texture2D normal;
	rend::Texture2D specular;

	void init(
		std::string aPath,
		std::string nPath,
		std::string sPath) {

		// Albedo
		rend::createTexture2D(
			albedo,
			aPath,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR,
			true);

		// Normal
		rend::createTexture2D(
			normal,
			nPath,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR,
			true);

		// Specular
		rend::createTexture2D(
			specular,
			sPath,
			GL_LINEAR_MIPMAP_LINEAR,
			GL_LINEAR,
			true);
	}

	void release() {
		albedo.release();
		normal.release();
		specular.release();
	}

	void bind() {
		albedo.bind(GL_TEXTURE0);
		normal.bind(GL_TEXTURE1);
		specular.bind(GL_TEXTURE2);
	}

	void unbind() {
		albedo.unbind(GL_TEXTURE0);
		normal.unbind(GL_TEXTURE1);
		specular.unbind(GL_TEXTURE2);
	}
};

static TextureMaterial grassTM;
static TextureMaterial dirtTM;
static TextureMaterial beachSandTM;
static TextureMaterial seaFloorTM;
static TextureMaterial brickTM;
static TextureMaterial waterTM;

static transform::Camera camera;

static float yrot = 0.0f;



void drawMesh(
	mesh::OpenGLMesh& mesh, 
	TextureMaterial& tex, 
	const glm::mat4& model);

void demo_init(ft::Table* table) {
	ftw::init(table);

	ftw::get()->input.createInputMapping("toggle-grab", ftw::get()->input.createInputMapKey(input::Keys::KEY_TAB));
	ftw::get()->input.createInputMapping("forward", ftw::get()->input.createInputMapKey(input::Keys::KEY_W));
	ftw::get()->input.createInputMapping("backward", ftw::get()->input.createInputMapKey(input::Keys::KEY_S));
	ftw::get()->input.createInputMapping("strafe-left", ftw::get()->input.createInputMapKey(input::Keys::KEY_A));
	ftw::get()->input.createInputMapping("strafe-right", ftw::get()->input.createInputMapKey(input::Keys::KEY_D));
	ftw::get()->input.createInputMapping("move-up", ftw::get()->input.createInputMapKey(input::Keys::KEY_SPACE));
	ftw::get()->input.createInputMapping("move-down", ftw::get()->input.createInputMapKey(input::Keys::KEY_LSHIFT));


	glEnable(GL_DEPTH_TEST);

	mainProg.init("data/shaders/main.vs.glsl", "data/shaders/main.fs.glsl");

	// Uniform Buffer
	matricesBuffer.init();
	matricesBuffer.update(&matrices, sizeof(matrices));
	matricesBuffer.bind(0, sizeof(matrices));

	lightBuffer.init();
	lightBuffer.update(&light, sizeof(light));
	lightBuffer.bind(1, sizeof(light));

	materialBuffer.init();
	materialBuffer.update(&material, sizeof(material));
	materialBuffer.bind(2, sizeof(material));

	// Load Mesh

	// Mesh
	floorMesh.init("data/meshes/floor.m");
	cubeMesh.init("data/meshes/cube.m");
	cylenderMesh.init("data/meshes/cylender.m");
	monkeyFaceMesh.init("data/meshes/monkey_face.m");
	sphereMesh.init("data/meshes/sphere.m");
	torusMesh.init("data/meshes/torus.m");

	// Texture Materials
	grassTM.init(
		"data/textures/grass.png",
		"data/textures/grass_normal.png",
		"data/textures/grass_specular.png"
	);

	dirtTM.init(
		"data/textures/dirt.png",
		"data/textures/dirt_normal.png",
		"data/textures/dirt_specular.png"
	);

	beachSandTM.init(
		"data/textures/beach_sand.png",
		"data/textures/beach_sand_normal.png",
		"data/textures/beach_sand_specular.png"
	);

	seaFloorTM.init(
		"data/textures/sea_floor.png",
		"data/textures/sea_floor_normal.png",
		"data/textures/sea_floor_specular.png"
	);

	brickTM.init(
		"data/textures/brick.png",
		"data/textures/brick_normal.png",
		"data/textures/brick_specular.png"
	);

	waterTM.init(
		"data/textures/water.png",
		"data/textures/water_normal.png",
		"data/textures/water_specular.png"
	);

	/*
	rend::createTexture2D(
		grassTex2D,
		"data/textures/grass.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);

	rend::createTexture2D(
		dirtTex2D,
		"data/textures/dirt.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);

	rend::createTexture2D(
		beachSandTex2D,
		"data/textures/beach_sand.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);

	rend::createTexture2D(
		seaFloorTex2D,
		"data/textures/sea_floor.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);

	rend::createTexture2D(
		brickTex2D,
		"data/textures/brick.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);

	rend::createTexture2D(
		waterTex2D,
		"data/textures/water.png",
		GL_LINEAR_MIPMAP_LINEAR,
		GL_LINEAR,
		true);
	*/
	camera.init(
		glm::vec3(0.0f, 2.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		65.0f,
		(float)ftw::get()->app.getWidth() / (float)ftw::get()->app.getHeight(),
		1.0f,
		1024.0f);


}

void demo_update(float delta) {

	if (ftw::get()->input.isInputMapPressOnce("toggle-grab")) {
		ftw::get()->input.toggleGrab();
	}

	camera.update(delta);

	yrot += 32.0f * delta;

	if (yrot > 360.0f) {
		yrot -= 360.0f;
	}
}

void demo_render() {

	matrices.proj = camera.toProj();
	matrices.view = camera.toView();

	matricesBuffer.update(&matrices, sizeof(matrices));


	// Setup Light
	light.direction = glm::vec3(0.0f, -0.5, 1.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(0.8f);
	light.specular = glm::vec3(1.0f);


	lightBuffer.update(&light, sizeof(light));

	glViewport(0, 0, ftw::get()->app.getWidth(), ftw::get()->app.getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProg.bind();

	mainProg.uniform3("cameraPos", camera.position.x, camera.position.y, camera.position.z);

	drawMesh(floorMesh, grassTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	drawMesh(cubeMesh, brickTM, glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(cylenderMesh, beachSandTM, glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(monkeyFaceMesh, seaFloorTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(sphereMesh, dirtTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(torusMesh, waterTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 9.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));

	mainProg.unbind();
}

void demo_release() {
	// Texture2D
	grassTM.release();
	dirtTM.release();
	beachSandTM.release();
	seaFloorTM.release();
	brickTM.release();
	waterTM.release();
	
	// Mesh
	floorMesh.release();
	cubeMesh.release();
	cylenderMesh.release();
	monkeyFaceMesh.release();
	sphereMesh.release();
	torusMesh.release();

	materialBuffer.release();
	lightBuffer.release();
	matricesBuffer.release();
	
	mainProg.release();

	ftw::get()->input.clearInputMaps();

	ftw::release();
}

void drawMesh(
	mesh::OpenGLMesh& mesh, 
	TextureMaterial& tex, 
	const glm::mat4& model) {

	mainProg.uniformMat4("model", model);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

	mainProg.uniformMat4("normalMatrix", normalMatrix);

	tex.bind();
	mainProg.bindAttributes();

	mesh.vertices.bind();
	mainProg.ptr("vertices", 3, GL_FLOAT);
	mesh.texCoords.bind();
	mainProg.ptr("texCoords", 2, GL_FLOAT);
	mesh.normals.bind();
	mainProg.ptr("normals", 3, GL_FLOAT);
	mesh.tangents.bind();
	mainProg.ptr("tangents", 3, GL_FLOAT);
	mesh.biTangents.bind();
	mainProg.ptr("biTangents", 3, GL_FLOAT);
	mesh.texCoords.unbind();

	mesh.index.bind();
	glDrawElements(GL_TRIANGLES, mesh.index.count(), GL_UNSIGNED_INT, nullptr);
	mesh.index.unbind();

	mainProg.unbindAttribute();
	tex.unbind();
}

