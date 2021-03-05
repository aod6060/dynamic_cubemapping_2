#include "library.h"


void demo_init(ft::Table* table) {
	ftw::init(table);

	glEnable(GL_DEPTH_TEST);
}

void demo_update(float delta) {

}

void demo_render() {
	glViewport(0, 0, ftw::get()->app.getWidth(), ftw::get()->app.getHeight());
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



}

void demo_release() {
	ftw::release();
}