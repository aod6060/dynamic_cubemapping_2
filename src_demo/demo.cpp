#include "library.h"

// Enviorment Mapping
#define ENV_REFLECT 0
#define ENV_REFRACT 1
#define ENV_GLASS 2
#define ENV_SIZE 3

// Effects
#define FX_REGULAR 0
#define FX_BK 1
#define FX_SEPAI 2
#define FX_INVERT 3
#define FX_BLUR 4
#define FX_PIXELATED 5
#define FX_SIZE 6

struct Matrices {
	glm::mat4 proj;
	glm::mat4 view;
	glm::mat4 lightSpaceMatrix;
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

	void init() {
		//this->init("data/shaders/main.vs.glsl", "data/shaders/main.fs.glsl");
		rend::Program::init(
			"data/shaders/main.vs.glsl", 
			"data/shaders/main.fs.glsl");
	}

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

		this->createUniform("shadowDepthMap");
		this->uniform1("shadowDepthMap", 3);

		this->createUniform("transMap");
		this->uniform1("transMap", 4);

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

struct SkyboxProgram : public rend::Program {
	
	void init() {
		rend::Program::init(
			"data/shaders/skybox.vs.glsl",
			"data/shaders/skybox.fs.glsl"
		);
	}

	virtual void onCreateUniformsAttributes() {
		// Uniforms
		this->createUniform("skybox");
		this->uniform1("skybox", 0);

		// Uniforms Buffer Object
		this->createUniformBlock("Matrices", 0);

		// Attribute
		this->createAttributes("vertices", 0);

		this->bindAttributes();
		this->enable("vertices");
		this->unbindAttribute();
	}
};

struct ShadowProgram : public rend::Program {

	void init() {
		rend::Program::init(
			"data/shaders/shadow.vs.glsl",
			"data/shaders/shadow.fs.glsl"
		);
	}

	virtual void onCreateUniformsAttributes() {
		// Uniforms
		this->createUniform("proj");
		this->createUniform("view");
		this->createUniform("model");
		this->createUniform("normalMatrix");
		this->createUniform("lightDir");
		this->createUniform("isTrans");
		this->createUniform("normalMap");
		this->uniform1("normalMap", 0);
		this->createUniform("isNormalMapped");

		// Attribute
		this->createAttributes("vertices", 0);
		this->createAttributes("normals", 1);
		this->createAttributes("tangents", 2);
		this->createAttributes("biTangents", 3);
		this->createAttributes("texCoords", 4);

		this->bindAttributes();
		this->enable("vertices");
		this->enable("normals");
		this->enable("tangents");
		this->enable("biTangents");
		this->enable("texCoords");

		this->unbindAttribute();
	}
};

struct HUBProgram : public rend::Program {
	void init() {
		rend::Program::init(
			"data/shaders/hub.vs.glsl",
			"data/shaders/hub.fs.glsl"
		);
	}

	virtual void onCreateUniformsAttributes() {
		// Uniform
		this->createUniform("proj");
		this->createUniform("view");
		this->createUniform("model");
		this->createUniform("tex0");
		this->uniform1("tex0", 0);
		// Attributes
		this->createAttributes("vertices", 0);
		this->createAttributes("texCoords", 1);

		this->bindAttributes();
		this->enable("vertices");
		this->enable("texCoords");
		this->unbindAttribute();
	}
};

struct ReflectiveProgram : public rend::Program {
	void init() {
		rend::Program::init(
			"data/shaders/reflective.vs.glsl",
			"data/shaders/reflective.fs.glsl"
		);
	}

	virtual void onCreateUniformsAttributes() {
		// Uniform
		this->createUniform("model");
		this->createUniform("normalMatrix");
		this->createUniform("cameraPos");

		this->createUniform("shadowMap");
		this->uniform1("shadowMap", 3);

		this->createUniform("transMap");
		this->uniform1("transMap", 4);

		this->createUniform("refTex");
		this->uniform1("refTex", 0);

		this->createUniform("normalMap");
		this->uniform1("normalMap", 1);

		this->createUniform("isNormalMapped");
		this->createUniform("envType");
		this->createUniform("fxType");

		// Uniform Buffer Object
		this->createUniformBlock("Matrices", 0);
		this->createUniformBlock("Light", 1);
		this->createUniformBlock("Material", 2);

		// Attribute
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

static SDL_GLContext context;

static Matrices matrices;
static Light light;
static Material material;

static MainProgram mainProg;
static SkyboxProgram skyboxProg;
static ShadowProgram shadowProg;
static HUBProgram hubProgram;
static ReflectiveProgram reflectiveProgram;

static rend::UniformBuffer matricesBuffer;
static rend::UniformBuffer lightBuffer;
static rend::UniformBuffer materialBuffer;

static mesh::OpenGLMesh floorMesh;
static mesh::OpenGLMesh cubeMesh;
static mesh::OpenGLMesh cylenderMesh;
static mesh::OpenGLMesh monkeyFaceMesh;
static mesh::OpenGLMesh sphereMesh;
static mesh::OpenGLMesh sphere2Mesh;
static mesh::OpenGLMesh torusMesh;

static std::vector<mesh::OpenGLMesh*> meshes;
static int32_t mIndex = 0;

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

static rend::Cubemap skyboxTex;

static float yrot = 0.0f;

const uint32_t SHADOW_SIZE = 1024;

rend::Texture2D depthMapTex;
rend::Texture2D transparentMap;
rend::Framebuffer depthMapFBO;

// HUB Buffers
rend::VertexBuffer hubVertices;
rend::VertexBuffer hubTexCoords;

bool isDepthShown = false;

// Dynamic Cubemap
const uint32_t DYNAMIC_CM_SIZE = 1024;

rend::Cubemap dynamicCubemap;
rend::Renderbuffer dynamicCubemapDepth;
rend::Framebuffer dynamicCubemapFB;


bool isNormalMapped = false;
int envType = ENV_REFLECT;
int fxType = FX_REGULAR;

void drawMesh(
	mesh::OpenGLMesh& mesh, 
	TextureMaterial& tex, 
	const glm::mat4& model);

void drawShadowMesh(
	mesh::OpenGLMesh& mesh,
	bool isTrans,
	const glm::mat4& model);

void drawSkybox(
	mesh::OpenGLMesh& mesh,
	rend::Cubemap& cubemap);

void demo_init(ft::Table* table) {
	ftw::init(table);

	// Init Context and glew
	context = SDL_GL_CreateContext(ftw::get()->app.getWindow());
	glewInit();

	ftw::get()->input.createInputMapping("toggle-grab", ftw::get()->input.createInputMapKey(input::Keys::KEY_TAB));
	ftw::get()->input.createInputMapping("forward", ftw::get()->input.createInputMapKey(input::Keys::KEY_W));
	ftw::get()->input.createInputMapping("backward", ftw::get()->input.createInputMapKey(input::Keys::KEY_S));
	ftw::get()->input.createInputMapping("strafe-left", ftw::get()->input.createInputMapKey(input::Keys::KEY_A));
	ftw::get()->input.createInputMapping("strafe-right", ftw::get()->input.createInputMapKey(input::Keys::KEY_D));
	ftw::get()->input.createInputMapping("move-up", ftw::get()->input.createInputMapKey(input::Keys::KEY_SPACE));
	ftw::get()->input.createInputMapping("move-down", ftw::get()->input.createInputMapKey(input::Keys::KEY_LSHIFT));
	ftw::get()->input.createInputMapping("toggle-depth", ftw::get()->input.createInputMapKey(input::Keys::KEY_1));
	ftw::get()->input.createInputMapping("next-mesh", ftw::get()->input.createInputMapKey(input::Keys::KEY_2));
	ftw::get()->input.createInputMapping("toggle-normalMap", ftw::get()->input.createInputMapKey(input::Keys::KEY_3));
	ftw::get()->input.createInputMapping("next-env-type", ftw::get()->input.createInputMapKey(input::Keys::KEY_4));
	ftw::get()->input.createInputMapping("next-fx-type", ftw::get()->input.createInputMapKey(input::Keys::KEY_5));

	glEnable(GL_DEPTH_TEST);

	
	mainProg.init();
	skyboxProg.init();
	shadowProg.init();
	hubProgram.init();
	reflectiveProgram.init();

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
	sphere2Mesh.init("data/meshes/sphere2.m");
	torusMesh.init("data/meshes/torus.m");

	meshes.push_back(&cubeMesh);
	meshes.push_back(&sphereMesh);
	meshes.push_back(&sphere2Mesh);
	meshes.push_back(&cylenderMesh);
	meshes.push_back(&torusMesh);
	meshes.push_back(&monkeyFaceMesh);

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
	camera.init(
		glm::vec3(8.0f, 8.0f, 8.0f),
		glm::vec2(45.0f, -45.0f),
		65.0f,
		(float)ftw::get()->app.getWidth() / (float)ftw::get()->app.getHeight(),
		1.0f,
		1024.0f);

	rend::createCubemap(
		skyboxTex,
		"data/textures/skybox/east.png",
		"data/textures/skybox/west.png",
		"data/textures/skybox/up.png",
		"data/textures/skybox/down.png",
		"data/textures/skybox/north.png",
		"data/textures/skybox/south.png");

	//frameBuffer.init();


	depthMapTex.init();
	depthMapTex.bind(GL_TEXTURE0);

	depthMapTex.update(SHADOW_SIZE, SHADOW_SIZE, GL_DEPTH_COMPONENT, GL_FLOAT);

	depthMapTex.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	depthMapTex.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	depthMapTex.parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	depthMapTex.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	depthMapTex.unbind(GL_TEXTURE0);

	transparentMap.init();
	transparentMap.bind(GL_TEXTURE0);
	transparentMap.update(SHADOW_SIZE, SHADOW_SIZE, GL_RGBA, GL_FLOAT);

	transparentMap.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	transparentMap.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	transparentMap.parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	transparentMap.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	transparentMap.unbind(GL_TEXTURE0);

	depthMapFBO.init();
	depthMapFBO.bind();

	depthMapFBO.attachDepthBuffer(depthMapTex);
	depthMapFBO.attachColorBuffer(GL_COLOR_ATTACHMENT0, transparentMap);

	depthMapFBO.drawBuffers(GL_COLOR_ATTACHMENT0);
	depthMapFBO.readBuffer(GL_COLOR_ATTACHMENT0);
	depthMapFBO.unbind();


	hubVertices.init();
	hubVertices.add3(0.0f, 0.0f, 0.0f);
	hubVertices.add3(1.0f, 0.0f, 0.0f);
	hubVertices.add3(0.0f, 1.0f, 0.0f);

	hubVertices.add3(0.0f, 1.0f, 0.0f);
	hubVertices.add3(1.0f, 0.0f, 0.0f);
	hubVertices.add3(1.0f, 1.0f, 0.0f);
	hubVertices.update();

	hubTexCoords.init();
	hubTexCoords.add2(0.0f, 1.0f);
	hubTexCoords.add2(1.0f, 1.0f);
	hubTexCoords.add2(0.0f, 0.0f);

	hubTexCoords.add2(0.0f, 0.0f);
	hubTexCoords.add2(1.0f, 1.0f);
	hubTexCoords.add2(1.0f, 0.0f);

	hubTexCoords.update();



	// Initialize Dynamic Mapping
	// Cubemap
	dynamicCubemap.init();

	dynamicCubemap.bind(GL_TEXTURE0);
	for (uint32_t i = 0; i < rend::Cubemap::SIZE; i++) {
		dynamicCubemap.update(i, GL_RGB, GL_UNSIGNED_BYTE, DYNAMIC_CM_SIZE, DYNAMIC_CM_SIZE);
	}

	dynamicCubemap.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	dynamicCubemap.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	dynamicCubemap.parameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	dynamicCubemap.parameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	dynamicCubemap.parameteri(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	dynamicCubemap.unbind(GL_TEXTURE0);

	// Renderbuffer
	dynamicCubemapDepth.init();

	dynamicCubemapDepth.bind();
	dynamicCubemapDepth.update(GL_DEPTH_COMPONENT, DYNAMIC_CM_SIZE, DYNAMIC_CM_SIZE);
	dynamicCubemapDepth.unbind();

	// Framebuffer
	dynamicCubemapFB.init();

	dynamicCubemapFB.bind();

	dynamicCubemapFB.attachDepthBuffer(dynamicCubemapDepth);

	dynamicCubemapFB.attachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, dynamicCubemap);

	dynamicCubemapFB.readBuffer(GL_COLOR_ATTACHMENT0);
	dynamicCubemapFB.drawBuffers(GL_COLOR_ATTACHMENT0);

	dynamicCubemapFB.unbind();
}

void demo_update(float delta) {

	if (ftw::get()->input.isInputMapPressOnce("toggle-grab")) {
		ftw::get()->input.toggleGrab();
	}

	if (ftw::get()->input.isInputMapPressOnce("toggle-depth")) {
		isDepthShown = !isDepthShown;
	}

	if (ftw::get()->input.isInputMapPressOnce("next-mesh")) {

		mIndex += 1;

		if (mIndex >= meshes.size()) {
			mIndex = 0;
		}
	}

	if (ftw::get()->input.isInputMapPressOnce("toggle-normalMap")) {
		isNormalMapped = !isNormalMapped;
	}

	if (ftw::get()->input.isInputMapPressOnce("next-env-type")) {
		envType += 1;

		if (envType >= ENV_SIZE) {
			envType = 0;
		}
	}

	if (ftw::get()->input.isInputMapPressOnce("next-fx-type")) {
		fxType += 1;
		if (fxType >= FX_SIZE) {
			fxType = 0;
		}
	}

	camera.update(delta);
		
	yrot += 32.0f * delta;

	if (yrot > 360.0f) {
		yrot -= 360.0f;
	}
}

void demo_render_scene() {
	skyboxProg.bind();
	glDepthMask(GL_FALSE);
	glDepthFunc(GL_LEQUAL);

	// Draw skybox
	drawSkybox(cubeMesh, skyboxTex);

	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	skyboxProg.unbind();

	depthMapTex.bind(GL_TEXTURE3);
	transparentMap.bind(GL_TEXTURE4);

	mainProg.bind();

	mainProg.uniform3("cameraPos", camera.position.x, camera.position.y, camera.position.z);

	
	drawMesh(floorMesh, grassTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	drawMesh(cubeMesh, brickTM, glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(cylenderMesh, beachSandTM, glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(monkeyFaceMesh, seaFloorTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(sphereMesh, dirtTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawMesh(torusMesh, waterTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 9.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	

	mainProg.unbind();

	reflectiveProgram.bind();

	reflectiveProgram.uniform1("isNormalMapped", isNormalMapped);
	reflectiveProgram.uniform1("envType", envType);
	reflectiveProgram.uniform1("fxType", fxType);

	dynamicCubemap.bind(GL_TEXTURE0);
	dirtTM.normal.bind(GL_TEXTURE1);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-yrot), glm::vec3(1.0f, 1.0f, 0.0f));

	reflectiveProgram.uniformMat4("model", model);

	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

	reflectiveProgram.uniformMat4("normalMatrix", normalMatrix);
	reflectiveProgram.uniform3("cameraPos", camera.position.x, camera.position.y, camera.position.z);

	reflectiveProgram.bindAttributes();

	meshes[mIndex]->vertices.bind();
	reflectiveProgram.ptr("vertices", 3, GL_FLOAT);
	meshes[mIndex]->texCoords.bind();
	reflectiveProgram.ptr("texCoords", 2, GL_FLOAT);
	meshes[mIndex]->normals.bind();
	reflectiveProgram.ptr("normals", 3, GL_FLOAT);
	meshes[mIndex]->tangents.bind();
	reflectiveProgram.ptr("tangents", 3, GL_FLOAT);
	meshes[mIndex]->biTangents.bind();
	reflectiveProgram.ptr("biTangents", 3, GL_FLOAT);
	meshes[mIndex]->vertices.unbind();

	meshes[mIndex]->index.bind();
	glDrawElements(GL_TRIANGLES, meshes[mIndex]->index.count(), GL_UNSIGNED_INT, nullptr);
	meshes[mIndex]->index.unbind();

	reflectiveProgram.unbindAttribute();

	reflectiveProgram.unbind();

	dirtTM.normal.bind(GL_TEXTURE1);
	dynamicCubemap.unbind(GL_TEXTURE0);
	transparentMap.unbind(GL_TEXTURE4);
	depthMapTex.unbind(GL_TEXTURE3);
}

void demo_render_shadow() {

	float size = 10.0f;
	float near_plane = 1.0f;
	float far_plane = 32.0f;

	//glEnable(GL_CULL_FACE);

	//glCullFace(GL_FRONT);
	//glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	depthMapFBO.bind();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glViewport(0, 0, SHADOW_SIZE, SHADOW_SIZE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 proj = glm::ortho(-size, size, -size, size, near_plane, far_plane);
	//glm::mat4 proj = camera.toProj();
	glm::mat4 view = glm::lookAt(-light.direction * size, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 view = camera.toView();

	matrices.lightSpaceMatrix = proj * view;

	shadowProg.bind();

	shadowProg.uniformMat4("proj", proj);
	shadowProg.uniformMat4("view", view);

	shadowProg.uniform3("lightDir", light.direction.x, light.direction.y, light.direction.z);

	shadowProg.uniform1("isNormalMapped", false);
	drawShadowMesh(floorMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
	drawShadowMesh(cubeMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawShadowMesh(cylenderMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawShadowMesh(monkeyFaceMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawShadowMesh(sphereMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	drawShadowMesh(torusMesh, false, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 9.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));

	shadowProg.uniform1("isNormalMapped", isNormalMapped);
	dirtTM.normal.bind(GL_TEXTURE0);
	drawShadowMesh(*meshes[mIndex], (envType == ENV_REFRACT || envType == ENV_GLASS) ? true : false, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(-yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
	dirtTM.normal.unbind(GL_TEXTURE0);

	shadowProg.unbind();
	
	depthMapFBO.unbind();
	//glCullFace(GL_BACK);

	//glDisable(GL_CULL_FACE);
}

void demo_render_cubemap() {
	std::function<void(uint32_t, glm::mat4&, glm::vec3)> toView = [&](uint32_t index, glm::mat4& out, glm::vec3 pos) {
		glm::mat4 la;

		switch (GL_TEXTURE_CUBE_MAP_POSITIVE_X + index) {
		case GL_TEXTURE_CUBE_MAP_POSITIVE_X:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(1, 0, 0),
				glm::vec3(0, -1, 0)
			);
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_X:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(-1, 0, 0),
				glm::vec3(0, -1, 0)
			);
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Y:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(0, 1, 0),
				glm::vec3(0, 0, 1)
			);
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(0, -1, 0),
				glm::vec3(0, 0, -1)
			);
			break;
		case GL_TEXTURE_CUBE_MAP_POSITIVE_Z:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(0, 0, 1),
				glm::vec3(0, -1, 0)
			);
			break;
		case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z:
			la = glm::lookAt(
				pos,
				pos + glm::vec3(0, 0, -1),
				glm::vec3(0, -1, 0)
			);
			break;
		}

		out = la;
	};


	for (uint32_t i = 0; i < rend::Cubemap::SIZE; i++) {
		dynamicCubemapFB.bind();
		dynamicCubemapFB.attachColorBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dynamicCubemap);

		Matrices m;

		m.proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 1024.0f);
		
		glm::mat4 view;
		toView(i, view, glm::vec3(0.0f, 2.0f, 0.0f));

		m.view = view;
		m.lightSpaceMatrix = matrices.lightSpaceMatrix;

		matricesBuffer.update(&m, sizeof(matrices));

		glViewport(0, 0, DYNAMIC_CM_SIZE, DYNAMIC_CM_SIZE);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skyboxProg.bind();
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);

		// Draw skybox
		drawSkybox(cubeMesh, skyboxTex);

		glDepthFunc(GL_LESS);
		glDepthMask(GL_TRUE);

		skyboxProg.unbind();

		depthMapTex.bind(GL_TEXTURE3);
		transparentMap.bind(GL_TEXTURE4);

		mainProg.bind();

		mainProg.uniform3("cameraPos", camera.position.x, camera.position.y, camera.position.z);


		drawMesh(floorMesh, grassTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
		drawMesh(cubeMesh, brickTM, glm::translate(glm::mat4(1.0f), glm::vec3(7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
		drawMesh(cylenderMesh, beachSandTM, glm::translate(glm::mat4(1.0f), glm::vec3(-7.0f, 2.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
		drawMesh(monkeyFaceMesh, seaFloorTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
		drawMesh(sphereMesh, dirtTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, -7.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));
		drawMesh(torusMesh, waterTM, glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 9.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(yrot), glm::vec3(1.0f, 1.0f, 0.0f)));


		mainProg.unbind();

		transparentMap.unbind(GL_TEXTURE4);
		depthMapTex.unbind(GL_TEXTURE3);

		dynamicCubemapFB.unbind();
	}

}

void render_hub() {
	glDisable(GL_DEPTH_TEST);

	glm::mat4 proj = glm::ortho(0.0f, (float)ftw::get()->app.getWidth(), (float)ftw::get()->app.getHeight(), 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 model =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(256.0f, 256.0f, 0.0f));



	hubProgram.bind();
	
	hubProgram.uniformMat4("proj", proj);
	hubProgram.uniformMat4("view", view);

	if (isDepthShown) {
		hubProgram.uniformMat4("model", model);

		transparentMap.bind(GL_TEXTURE0);
		hubProgram.bindAttributes();

		hubVertices.bind();
		hubProgram.ptr("vertices", 3, GL_FLOAT);
		hubTexCoords.bind();
		hubProgram.ptr("texCoords", 2, GL_FLOAT);
		hubTexCoords.unbind();

		glDrawArrays(GL_TRIANGLES, 0, hubVertices.count());

		hubProgram.unbindAttribute();
		transparentMap.unbind(GL_TEXTURE0);
	}

	hubProgram.unbind();

	glEnable(GL_DEPTH_TEST);
}

void demo_render() {

	// Setup Light
	light.direction = glm::vec3(1.0f, -1.0f, -1.0f);
	light.ambient = glm::vec3(0.1f);
	light.diffuse = glm::vec3(0.8f);
	light.specular = glm::vec3(1.0f);


	lightBuffer.update(&light, sizeof(light));

	demo_render_shadow();


	demo_render_cubemap();

	matrices.proj = camera.toProj();
	matrices.view = camera.toView();

	matricesBuffer.update(&matrices, sizeof(matrices));


	// Handle Framebuffers
	glViewport(0, 0, ftw::get()->app.getWidth(), ftw::get()->app.getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	demo_render_scene();

	render_hub();
}

void demo_release() {

	dynamicCubemapFB.release();
	dynamicCubemapDepth.release();
	dynamicCubemap.release();

	hubTexCoords.release();
	hubVertices.release();

	//hubProgram.release();

	// Framebuffer
	//frameBuffer.release();

	depthMapFBO.release();
	transparentMap.release();
	depthMapTex.release();

	// Skybox
	skyboxTex.release();
	// Texture2D
	grassTM.release();
	dirtTM.release();
	beachSandTM.release();
	seaFloorTM.release();
	brickTM.release();
	waterTM.release();
	
	// Mesh
	meshes.clear();

	floorMesh.release();
	cubeMesh.release();
	cylenderMesh.release();
	monkeyFaceMesh.release();
	sphereMesh.release();
	sphere2Mesh.release();
	torusMesh.release();

	materialBuffer.release();
	lightBuffer.release();
	matricesBuffer.release();
	
	reflectiveProgram.release();
	hubProgram.release();
	shadowProg.release();
	skyboxProg.release();
	mainProg.release();

	ftw::get()->input.clearInputMaps();

	SDL_GL_DeleteContext(context);

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

void drawShadowMesh(mesh::OpenGLMesh& mesh, bool isTrans, const glm::mat4& model) {
	shadowProg.uniformMat4("model", model);
	
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

	shadowProg.uniformMat4("normalMatrix", normalMatrix);
	shadowProg.uniform1("isTrans", isTrans);

	shadowProg.bindAttributes();

	mesh.vertices.bind();
	shadowProg.ptr("vertices", 3, GL_FLOAT);
	mesh.normals.bind();
	shadowProg.ptr("normals", 3, GL_FLOAT);
	mesh.tangents.bind();
	shadowProg.ptr("tangents", 3, GL_FLOAT);
	mesh.biTangents.bind();
	shadowProg.ptr("biTangents", 3, GL_FLOAT);
	mesh.texCoords.bind();
	shadowProg.ptr("texCoords", 2, GL_FLOAT);
	mesh.vertices.unbind();

	mesh.index.bind();
	glDrawElements(GL_TRIANGLES, mesh.index.count(), GL_UNSIGNED_INT, nullptr);
	mesh.index.unbind();

	shadowProg.unbindAttribute();
}

void drawSkybox(
	mesh::OpenGLMesh& mesh,
	rend::Cubemap& cubemap) {

	cubemap.bind(GL_TEXTURE0);

	skyboxProg.bindAttributes();

	mesh.vertices.bind();
	skyboxProg.ptr("vertices", 3, GL_FLOAT);
	
	mesh.vertices.unbind();

	mesh.index.bind();
	glDrawElements(GL_TRIANGLES, mesh.index.count(), GL_UNSIGNED_INT, nullptr);
	mesh.index.unbind();

	skyboxProg.unbindAttribute();

	cubemap.unbind(GL_TEXTURE0);
}