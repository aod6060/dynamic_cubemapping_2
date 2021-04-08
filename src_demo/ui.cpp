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
		if (str.size() > 0) {
			TTF_SizeText(font, str.c_str(), &width, &height);
		}
		else {
			width = 0;
			height = 0;
		}
	}

	void renderText(std::string str) {
		if (str.size() > 0) {
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
	}

	void unbind() {
		uiProgram.unbind();
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}



	void Rect::init(glm::vec2 position, glm::vec2 size) {
		this->position = position;
		this->size = size;
	}

	float Rect::left() {
		return this->position.x;
	}

	float Rect::right() {
		return this->position.x + this->size.x;
	}

	float Rect::top() {
		return this->position.y;
	}

	float Rect::bottom() {
		return this->position.y + this->size.y;
	}

	bool Rect::isCollision(Rect& r) {
		return
			this->left() < r.right() &&
			this->right() > r.left() &&
			this->top() < r.bottom() &&
			this->bottom() > r.top();
	}

	bool Rect::isCollision(glm::vec2& v) {
		return
			this->left() < v.x &&
			this->right() > v.x &&
			this->top() < v.y &&
			this->bottom() > v.y;
	}

	// Label
	void Label::init() {
		// Not sure
	}

	void Label::doEvent(SDL_Event& e) {
		// Not sure
	}

	void Label::update(float delta) {
		// Not Sure
	}

	void Label::render() {


		// Render Background
		int width, height;
		ui::textSize(this->text, width, height);
		this->size = glm::vec2(width, height);
		ui::setColor(this->color);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
		);
		ui::renderText(this->text);
	}

	void Label::release() {
		// Not sure
	}

	glm::vec2 Label::getPosition() {
		return this->position;
	}

	void Label::setPosition(glm::vec2 pos) {
		this->position = pos;
	}

	glm::vec2 Label::getSize() {
		return this->size;
	}

	void Label::setSize(glm::vec2 size) {
		this->size = size;
	}

	glm::vec4 Label::getColor() {
		return this->color;
	}

	void Label::setColor(glm::vec4 color) {
		this->color = color;
	}

	void Label::setText(std::string text) {
		this->text = text;
	}

	std::string Label::getText() {
		return this->text;
	}



	// Button
	void Button::init() {

	}

	void Button::doEvent(SDL_Event& e) {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				if (!this->isActivated) {
					Rect r;
					r.init(this->position, this->size);

					glm::vec2 mc = glm::vec2(e.button.x, e.button.y);

					if (r.isCollision(mc)) {

						if (this->actionCB) {
							this->actionCB(this);
						}

						this->isActivated = true;
						this->time = this->maxTime;
					}
				}
			}
		}
	}

	void Button::update(float delta) {

		if (this->isActivated) {
			if (this->time <= 0.0f) {
				this->isActivated = false;
			}
			else {
				this->time -= delta;
			}
		}
	}

	void Button::render() {
		int width, height;
		ui::textSize(this->text, width, height);
		this->size = glm::vec2(width, height);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
		);

		// Background
		if (this->isActivated) {
			ui::setColor(this->activeColor);
		}
		else {
			ui::setColor(this->backgroundColor);
		}
		ui::render();

		// Foreground
		ui::setColor(this->color);
		ui::renderText(this->text);
	}

	void Button::release() {

	}

	glm::vec2 Button::getPosition() {
		return this->position;
	}

	void Button::setPosition(glm::vec2 pos) {
		this->position = pos;
	}

	glm::vec2 Button::getSize() {
		return this->size;
	}

	void Button::setSize(glm::vec2 size) {
		this->size = size;
	}

	glm::vec4 Button::getColor() {
		return this->color;
	}

	void Button::setColor(glm::vec4 color) {
		this->color = color;
	}

	glm::vec4 Button::getBackgroundColor() {
		return this->backgroundColor;
	}

	void Button::setBackgroundColor(glm::vec4 bg) {
		this->backgroundColor = bg;
	}

	glm::vec4 Button::getActiveColor() {
		return this->activeColor;
	}

	void Button::setActiveColor(glm::vec4 ac) {
		this->activeColor = ac;
	}

	void Button::setAction(std::function<void(IAction*)> actionCB) {
		this->actionCB = actionCB;
	}

	void Button::setText(std::string text) {
		this->text = text;
	}

	std::string Button::getText() {
		return this->text;
	}

	// CheckBox
	void CheckBox::init() {

	}

	void CheckBox::doEvent(SDL_Event& e) {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				Rect r;

				r.init(this->position, this->size);

				glm::vec2 mc = glm::vec2(e.button.x, e.button.y);

				if (r.isCollision(mc)) {
					this->checked = !this->checked;

					if (this->actionCB) {
						this->actionCB(this);
					}
				}


			}
		}
	}

	void CheckBox::update(float delta) {

	}

	void CheckBox::render() {
		int width, height;
		ui::textSize(this->text, width, height);
		glm::vec2 size = glm::vec2(width, height);

		// Render Background
		ui::setColor(this->backgroundColor);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2(height, height), 0.0f))
		);

		ui::render();

		// Render Switch
		if (this->checked) {
			ui::setColor(this->onColor);
		}
		else {
			ui::setColor(this->offColor);
		}

		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position + glm::vec2(4.0f, 4.0f), 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2(height, height) - glm::vec2(4.0f, 4.0f) * 2.0f, 0.0f))
		);
		ui::render();

		// Render Forground


		ui::setColor(this->color);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position + glm::vec2(height + 1, 0), 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f))
		);
		ui::renderText(this->text);

		this->size = size + glm::vec2(height, 0.0f);
	}

	void CheckBox::release() {

	}

	glm::vec2 CheckBox::getPosition() {
		return this->position;
	}

	void CheckBox::setPosition(glm::vec2 pos) {
		this->position = pos;
	}

	glm::vec2 CheckBox::getSize() {
		return this->size;
	}

	void CheckBox::setSize(glm::vec2 size) {
		this->size = size;
	}

	glm::vec4 CheckBox::getColor() {
		return this->color;
	}

	void CheckBox::setColor(glm::vec4 color) {
		this->color = color;
	}

	void CheckBox::setAction(std::function<void(IAction*)> actionCB) {
		this->actionCB = actionCB;
	}

	void CheckBox::setText(std::string text) {
		this->text = text;
	}

	std::string CheckBox::getText() {
		return this->text;
	}

	bool CheckBox::isChecked() {
		return this->checked;
	}

	void CheckBox::setChecked(bool checked) {
		this->checked = checked;
	}

	glm::vec4 CheckBox::getOffColor() {
		return this->offColor;
	}
	
	void CheckBox::setOffColor(glm::vec4 color) {
		this->offColor = color;
	}

	glm::vec4 CheckBox::getOnColor() {
		return this->onColor;
	}
	
	void CheckBox::setOnColor(glm::vec4 color) {
		this->onColor = color;
	}

	glm::vec4 CheckBox::getBackgroundColor() {
		return this->backgroundColor;
	}

	void CheckBox::setBackgroundColor(glm::vec4 color) {
		this->backgroundColor = color;
	}


	// Select Button
	void SelectButton::init() {
	}

	void SelectButton::doEvent(SDL_Event& e) {

		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				Rect r;

				r.init(this->position, this->size);

				glm::vec2 mc = glm::vec2(e.button.x, e.button.y);

				if (r.isCollision(mc)) {
					this->currentValue += 1;

					if (this->currentValue >= this->values.size()) {
						this->currentValue = 0;
					}

					if (this->actionCB) {
						this->actionCB(this);
					}
				}
			}
		}
	}

	void SelectButton::update(float delta) {
	}

	void SelectButton::render() {
		int width, height;
		ui::textSize(this->getCurrentValue()->name, width, height);
		glm::vec2 size = glm::vec2(width, height);

		// Render Background
		ui::setColor(this->backgroundColor);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2(height, height), 0.0f))
		);

		ui::render();

		ui::setColor(this->color);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position + glm::vec2(height + 1, 0), 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f))
		);
		ui::renderText(this->getCurrentValue()->name);

		this->size = size + glm::vec2(height, 0.0f);
	}

	void SelectButton::release() {
	}

	glm::vec2 SelectButton::getPosition() {
		return this->position;
	}

	void SelectButton::setPosition(glm::vec2 p) {
		this->position = p;
	}

	glm::vec2 SelectButton::getSize() {
		return this->size;
	}

	void SelectButton::setSize(glm::vec2 s) {
		this->size = s;
	}

	glm::vec4 SelectButton::getColor() {
		return this->color;
	}

	void SelectButton::setColor(glm::vec4 c) {
		this->color = c;
	}

	void SelectButton::setAction(std::function<void(IAction*)> actionCB) {
		this->actionCB = actionCB;
	}

	SelectButton::SelectButtonValue* SelectButton::getCurrentValue() {
		return &this->values[this->currentValue];
	}

	void SelectButton::setBackgroundColor(glm::vec4 c) {
		this->backgroundColor = c;
	}
	
	glm::vec4 SelectButton::getBackgroundColor() {
		return this->backgroundColor;
	}

	void SelectButton::add(const SelectButtonValue& value) {
		this->values.push_back(value);
	}

	void SelectButton::clear() {
		this->values.clear();
	}

	// Slider
	void Slider::init() {

	}

	void Slider::doEvent(SDL_Event& e) {
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				isMoving = true;
			}
		}
		else if(e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				isMoving = false;
			}
		}
	}

	void Slider::update(float delta) {

		if (isMoving) {

			Rect r;

			int min = 0;
			int max = 100;

			input::MouseCoord mc;


			ftw::get()->input.getMousePosition(mc);

			glm::vec2 mcv = glm::vec2(mc.x, mc.y);

			r.init(this->position - glm::vec2(1.0f, 0.0f), glm::vec2(size.x, 16.0f));

			if (r.isCollision(mcv)) {
				if (min + position.x <= mc.x && max + position.x >= mc.x) {
					this->nobX = mc.x;


					float x = glm::abs(this->nobX - position.x);

					value = x / 100.0f;

					if (this->actionCB) {
						this->actionCB(this);
					}
				}
			}
		}

	}

	void Slider::render() {
		std::stringstream ss;

		ss << this->getValue();

		int width, height;
		ui::textSize(ss.str(), width, height);
		glm::vec2 size = glm::vec2(width, height);

		glm::vec2 sliderSize = glm::vec2(100.0f, 16.0f);

		glm::vec2 nobSize = glm::vec2(8.0f, 16.0);

		// Render Background
		ui::setColor(this->backgroundColor);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(sliderSize, 0.0f))
		);
		ui::render();

		// Render Nob
		ui::setColor(this->nobColor);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(nobX, position.y), 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(nobSize, 0.0f))
		);
		ui::render();

		ui::setColor(this->color);
		ui::setModel(
			glm::translate(glm::mat4(1.0f), glm::vec3(this->position + glm::vec2(sliderSize.x + nobSize.x, 0.0f), 0.0f)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size, 0.0f))
		);
		ui::renderText(ss.str());

		this->size = size + sliderSize;

	}

	void Slider::release() {

	}

	glm::vec2 Slider::getPosition() {
		return this->position;
	}

	void Slider::setPositions(glm::vec2 size) {
		this->position = size;
		this->nobX = this->position.x;
	}

	glm::vec2 Slider::getSize() {
		return this->size;
	}

	void Slider::setSize(glm::vec2 size) {
		this->size = size;
	}

	glm::vec4 Slider::getColor() {
		return this->color;
	}

	void Slider::setColor(glm::vec4 color) {
		this->color = color;
	}

	void Slider::setAction(std::function<void(IAction*)> actionCB) {
		this->actionCB = actionCB;
	}

	glm::vec4 Slider::getBackgroundColor() {
		return this->backgroundColor;
	}

	void Slider::setBackgroundColor(glm::vec4 bgColor) {
		this->backgroundColor = bgColor;
	}

	glm::vec4 Slider::getNobColor() {
		return this->nobColor;
	}

	void Slider::setNobColor(glm::vec4 nobColor) {
		this->nobColor = nobColor;
	}

	float Slider::getValue() {
		return minRange + value * (maxRange - minRange);
	}

	void Slider::setMinRange(float minRange) {
		this->minRange = minRange;
	}

	void Slider::setMaxRange(float maxRange) {
		this->maxRange = maxRange;
	}

	// UIContainer
	void UIContainer::init() {
		std::for_each(comps.begin(), comps.end(), [&](IComponent* comp) {
			comp->init();
		});
	}

	void UIContainer::doEvent(SDL_Event& e) {
		if (this->active) {
			std::for_each(comps.begin(), comps.end(), [&](IComponent* comp) {
				comp->doEvent(e);
			});
		}
	}

	void UIContainer::update(float delta) {
		if (this->active) {
			std::for_each(comps.begin(), comps.end(), [&](IComponent* comp) {
				comp->update(delta);
			});
		}
	}

	void UIContainer::render() {
		if (this->active) {
			glDisable(GL_DEPTH_TEST);
			ui::bind();
			
			ui::setProjection(glm::ortho(0.0f, (float)ftw::get()->app.getWidth(), (float)ftw::get()->app.getHeight(), 0.0f));
			ui::setView(glm::mat4(1.0f));

			ui::setColor(this->color);

			ui::setModel(
				glm::translate(glm::mat4(1.0f), glm::vec3(this->position, 0.0f)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(this->size, 0.0f))
			);

			ui::render();

			std::for_each(comps.begin(), comps.end(), [&](IComponent* comp) {
				comp->render();
			});
			ui::unbind();
			glEnable(GL_DEPTH_TEST);
		}
	}

	void UIContainer::release() {
		std::for_each(comps.begin(), comps.end(), [&](IComponent* comp) {
			comp->release();
		});
	}

	glm::vec2 UIContainer::getPosition() {
		return this->position;
	}

	void UIContainer::setPosition(glm::vec2 position) {
		this->position = position;
	}

	glm::vec2 UIContainer::getSize() {
		return this->size;
	}

	void UIContainer::setSize(glm::vec2 size) {
		this->size = size;
	}

	glm::vec4 UIContainer::getColor() {
		return this->color;
	}

	void UIContainer::setColor(glm::vec4 color) {
		this->color = color;
	}

	void UIContainer::add(IComponent* comp) {
		comps.push_back(comp);
	}

	void UIContainer::remove(IComponent* comp) {
		int index = -1;

		for (int i = 0; i < this->comps.size(); i++) {
			if (comps[i] == comp) {
				index = i;
			}
		}

		if (index != -1) {
			comps.erase(comps.begin() + index);
		}
	}

	uint32_t UIContainer::count() {
		return this->comps.size();
	}

	bool UIContainer::isActive() {
		return this->active;
	}

	void UIContainer::setActive(bool active) {
		this->active = active;
	}

}

