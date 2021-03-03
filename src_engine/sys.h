#pragma once




#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <random>
#include <chrono>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <GL/glew.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace app {
	struct RTPConfig {
		// A Check to see if it is in developer mode
		bool isDeveloper;
		// Modules
		std::string module_dll;
		std::string module_test_dll;
		// Build File
		std::string build_file;
	};

	struct Config {
		std::string caption;
		uint32_t width;
		uint32_t height;

		RTPConfig rtp_config;
	};

	void init(Config* config);
	void update();
	void release();

	uint32_t getWidth();
	uint32_t getHeight();

	void exit();
}

namespace ft {
	struct Table {};

	void init(Table* table);
	void release(Table* table);
}