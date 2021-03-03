#include "sys.h"

int main(int argc, char** argv) {

	app::Config conf;

	conf.caption = "Dynamic Cubmapping 2";
	conf.width = 1280;
	conf.height = 720;

	conf.rtp_config.isDeveloper = true;
	conf.rtp_config.module_dll = "demo.dll";
	conf.rtp_config.module_test_dll = "demo_test.dll";
	conf.rtp_config.build_file = "build.bin";

	app::init(&conf);
	app::update();
	app::release();

	return 0;
}