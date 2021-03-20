#include "library.h"


namespace ui {

	struct UIProgram : public rend::Program {
		void init() {
			rend::Program::init(
				"data/shaders/ui.vs.glsl",
				"data/shaders/ui.fs.glsl");
		}

		virtual void onCreateUniformsAttributes() {
			// Uniform
			this->createUniform("proj");
			this->createUniform("view");
			this->createUniform("model");
			this->createUniform("tex0");
			this->uniform1("tex0", 0);
			this->createUniform("color");

			// Attribute
			this->createAttributes("vertices", 0);
			this->createAttributes("texCoords", 1);

			this->bindAttributes();
			this->enable("vertices");
			this->enable("texCoords");
			this->unbindAttribute();
		}
	} uiProgram;

	TTF_Font* font = nullptr;

	rend::VertexBuffer vertices;
	rend::VertexBuffer texCoords;
	rend::IndexBuffer index;

	rend::Texture2D blank;
	rend::Texture2D blank50;

	rend::Texture2D fontTex;

	void init() {
		TTF_Init();

		font = TTF_OpenFont("data/font/font.ttf", 16);

		uiProgram.init();

		vertices.init();
		vertices.add3(0.0f, 0.0f, 0.0f);
		vertices.add3(1.0f, 0.0f, 0.0f);
		vertices.add3(0.0f, 1.0f, 0.0f);
		vertices.add3(1.0f, 1.0f, 0.0f);
		vertices.update();

		texCoords.init();
		texCoords.add2(0.0f, 0.0f);
		texCoords.add2(1.0f, 0.0f);
		texCoords.add2(0.0f, 1.0f);
		texCoords.add2(1.0f, 1.0f);
		texCoords.update();

		index.init();
		index.add3(0, 1, 2);
		index.add3(2, 1, 3);
		index.update();

		rend::createTexture2D(blank, "data/textures/blank.png", GL_LINEAR, GL_LINEAR, false);
		rend::createTexture2D(blank50, "data/textures/blank50.png", GL_LINEAR, GL_LINEAR, false);

		fontTex.init();

		fontTex.bind(GL_TEXTURE0);

		fontTex.update(1, 1, GL_RGBA, GL_UNSIGNED_BYTE);

		fontTex.parameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		fontTex.parameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		fontTex.unbind(GL_TEXTURE0);
	}

	void release() {
		blank50.release();
		blank.release();

		index.release();
		texCoords.release();
		vertices.release();

		uiProgram.release();

		TTF_CloseFont(font);

		TTF_Quit();
	}

	void bind() {
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		uiProgram.bind();
	}

	void setProjection(const glm::mat4& m) {
		uiProgram.uniformMat4("proj", m);
	}

	void setView(const glm::mat4& m) {
		uiProgram.uniformMat4("view", m);
	}

	void setModel(const glm::mat4& m) {
		uiProgram.uniformMat4("model", m);
	}

	void setColor(const glm::vec4& color) {
		uiProgram.uniform4("color", color.x, color.y, color.z, color.a);
	}

	void render() {
		blank.bind(GL_TEXTURE0);
		uiProgram.bindAttributes();

		vertices.bind();
		uiProgram.ptr("vertices", 3, GL_FLOAT);
		texCoords.bind();
		uiProgram.ptr("texCoords", 2, GL_FLOAT);
		texCoords.unbind();

		index.bind();
		glDrawElements(GL_TRIANGLES, index.count(), GL_UNSIGNED_INT, nullptr);
		index.unbind();

		uiProgram.unbindAttribute();
		blank.unbind(GL_TEXTURE0);
	}

	void render50() {
		blank50.bind(GL_TEXTURE0);
		uiProgram.bindAttributes();

		vertices.bind();
		uiProgram.ptr("vertices", 3, GL_FLOAT);
		texCoords.bind();
		uiProgram.ptr("texCoords", 2, GL_FLOAT);
		texCoords.unbind();

		index.bind();
		glDrawElements(GL_TRIANGLES, index.count(), GL_UNSIGNED_INT, nullptr);
		index.unbind();

		uiProgram.unbindAttribute();
		blank50.unbind(GL_TEXTURE0);
	}

	void textSize(std::string str, int32_t& width, int32_t& height) {
		TTF_SizeText(font, str.c_str(), &width, &height);
	}

	void renderText(std::string str) {
		SDL_Color c;
		c.r = 255;
		c.g = 255;
		c.b = 255;
		c.a = 255;

		SDL_Surface* temp = TTF_RenderText_Blended(font, str.c_str(), c);

		fontTex.bind(GL_TEXTURE0);

		fontTex.update(temp);

		uiProgram.bindAttributes();

		vertices.bind();
		uiProgram.ptr("vertices", 3, GL_FLOAT);
		texCoords.bind();
		uiProgram.ptr("texCoords", 2, GL_FLOAT);
		texCoords.unbind();

		index.bind();
		glDrawElements(GL_TRIANGLES, index.count(), GL_UNSIGNED_INT, nullptr);
		index.unbind();

		uiProgram.unbindAttribute();

		fontTex.unbind(GL_TEXTURE0);
		SDL_FreeSurface(temp);
	}

	void unbind() {
		uiProgram.unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
}

