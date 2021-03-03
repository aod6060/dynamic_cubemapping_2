#include "sys.h"


namespace app {
	typedef void(*module_init_def)(ft::Table*);
	typedef void(*module_update_def)(float);
	typedef void(*module_render_def)();
	typedef void(*module_release_def)();

	Config* _conf = nullptr;
	bool _isRunning = true;
	
	SDL_Window* _window = nullptr;
	SDL_GLContext _context = nullptr;


	// Library
	struct Library {
		HINSTANCE library = nullptr;
		module_init_def init;
		module_update_def update;
		module_render_def render;
		module_release_def release;
	} _library;

	// Function Table
	ft::Table _func_table;

	// Refreshing Section
	struct BuildRefresh {
		uint64_t version = 0;
		bool needsRefresh = false;
		float timer = 0.0f;
		float maxTimer = 1.0f;
	} _buildRef;

	void init(Config* config) {
		_conf = config;

		// Setup OpenGL
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_Init(SDL_INIT_EVERYTHING);

		// Create Window
		_window = SDL_CreateWindow(
			_conf->caption.c_str(),
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			_conf->width,
			_conf->height,
			SDL_WINDOW_OPENGL);


		// Create Context
		_context = SDL_GL_CreateContext(_window);
		glewInit();


		ft::init(&_func_table);

		if (_conf->rtp_config.isDeveloper) {
			// Implement Later
		}
		else {
			_library.library = LoadLibrary(_conf->rtp_config.module_dll.c_str());

			_library.init = (module_init_def)GetProcAddress(_library.library, "demo_init");
			_library.update = (module_update_def)GetProcAddress(_library.library, "demo_update");
			_library.render = (module_render_def)GetProcAddress(_library.library, "demo_render");
			_library.release = (module_release_def)GetProcAddress(_library.library, "demo_release");
		}

		if (_library.init) {
			_library.init(&_func_table);
		}
	}

	void update() {
		SDL_Event e;

		uint32_t pre = SDL_GetTicks();
		uint32_t curr = 0;
		float delta = 0.0f;

		while (_isRunning) {
			curr = SDL_GetTicks();
			delta = (curr - pre) / 1000.0f;
			pre = curr;

			while (SDL_PollEvent(&e)) {
				if (e.type == SDL_QUIT) {
					_isRunning = false;
				}
			}

			if (_library.update) {
				_library.update(delta);
			}

			if (_library.render) {
				_library.render();
			}

			SDL_GL_SwapWindow(_window);
		}
	}

	void release() {

		if (_library.release) {
			_library.release();
		}

		FreeLibrary(_library.library);

		_library.init = nullptr;
		_library.update = nullptr;
		_library.render = nullptr;
		_library.release = nullptr;

		SDL_GL_DeleteContext(_context);
		SDL_DestroyWindow(_window);
		SDL_Quit();
	}

	uint32_t getWidth() {
		return _conf->width;
	}

	uint32_t getHeight() {
		return _conf->height;
	}

	void exit() {
		_isRunning = false;
	}

}