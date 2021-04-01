#include "library.h"


namespace transform {


	void Camera::init(
		glm::vec3 position,
		glm::vec2 rotation,
		float fov,
		float aspect,
		float zmin,
		float zmax) {

		this->position = position;
		this->rotation = rotation;
		this->fov = fov;
		this->aspect = aspect;
		this->zmin = zmin;
		this->zmax = zmax;
	}

	void Camera::update(float delta) {

		if (ftw::get()->input.isGrab()) {

			input::MouseCoord v;

			ftw::get()->input.getMouseVelocity(v);

			this->rotation.x += v.y * this->rot_speed * ((delta > 0.001f) ? delta : 0.001f);
			this->rotation.y += v.x * this->rot_speed * ((delta > 0.001f) ? delta : 0.001f);

			if (this->rotation.x < -90.0f) {
				this->rotation.x = -90.0f;
			}

			if (this->rotation.x > 90.0f) {
				this->rotation.x = 90.0f;
			}

			if (this->rotation.y < -360.0f) {
				this->rotation.y += 360.0f;
			}

			if (this->rotation.y > 360.0f) {
				this->rotation.y -= 360.0f;
			}


			float yrad = glm::radians(this->rotation.y);

			float speed = this->walk_speed;

			if (ftw::get()->input.isInputMapPress("forward")) {
				this->position.x += glm::sin(yrad) * delta * speed;
				this->position.z -= glm::cos(yrad) * delta * speed;
			}

			if (ftw::get()->input.isInputMapPress("backward")) {
				this->position.x -= glm::sin(yrad) * delta * speed;
				this->position.z += glm::cos(yrad) * delta * speed;
			}

			if (ftw::get()->input.isInputMapPress("strafe-left")) {
				this->position.x -= glm::cos(yrad) * delta * speed;
				this->position.z -= glm::sin(yrad) * delta * speed;
			}

			if (ftw::get()->input.isInputMapPress("strafe-right")) {
				this->position.x += glm::cos(yrad) * delta * speed;
				this->position.z += glm::sin(yrad) * delta * speed;
			}

			if (ftw::get()->input.isInputMapPress("move-up")) {
				this->position.y += delta * speed;
			}

			if (ftw::get()->input.isInputMapPress("move-down")) {
				this->position.y -= delta * speed;
			}
		}

	}

	glm::mat4 Camera::toProj() {
		return glm::perspective(
			glm::radians(this->fov),
			this->aspect,
			this->zmin,
			this->zmax);
	}

	glm::mat4 Camera::toView() {
		return
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::translate(glm::mat4(1.0f), -this->position);
	}


	// Path
	void Path::add(PathData data) {
		this->pathData.push_back(data);
	}

	void Path::clear() {
		this->pathData.clear();
	}

	void Path::update(float delta) {
		if (time > maxTime) {
			time = 0.0f;
			this->index += 1;

			if (this->index >= this->pathData.size()) {
				this->index = 0;
			}
		}
		else {
			time += delta;
		}
	}

	glm::vec3 Path::getPosition() {
		glm::vec3 min_p = this->pathData[index].position;
		glm::vec3 max_p;

		if (index + 1 >= this->pathData.size()) {
			max_p = this->pathData[0].position;
		}
		else {
			max_p = this->pathData[index + 1].position;
		}

		return this->l(min_p, max_p, time);
	}

	glm::vec3 Path::l(glm::vec3 min, glm::vec3 max, float t) {
		return (max - min) * t + min;
	}
}