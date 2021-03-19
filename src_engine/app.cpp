#include "sys.h"


namespace app {
	typedef void(*module_init_def)(ft::Table*);
	typedef void(*module_update_def)(float);
	typedef void(*module_render_def)();
	typedef void(*module_release_def)();

	Config* _conf = nullptr;
	bool _isRunning = true;
	
	SDL_Window* _window = nullptr;
	//SDL_GLContext _context = nullptr;


	// Library
	struct Library {
		HINSTANCE library = nullptr;
		module_init_def init = nullptr;
		module_update_def update = nullptr;
		module_render_def render = nullptr;
		module_release_def release = nullptr;

		void initLibrary(std::string path) {
			this->library = LoadLibrary(path.c_str());

			this->init = (module_init_def)GetProcAddress(_library.library, "demo_init");
			this->update = (module_update_def)GetProcAddress(_library.library, "demo_update");
			this->render = (module_render_def)GetProcAddress(_library.library, "demo_render");
			this->release = (module_release_def)GetProcAddress(_library.library, "demo_release");

		}

		void releaseLibrary() {
			FreeLibrary(this->library);

			this->init = nullptr;
			this->update = nullptr;
			this->render = nullptr;
			this->release = nullptr;

		}

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
		//_context = SDL_GL_CreateContext(_window);
		//glewInit();

		input::init();

		ft::init(&_func_table);

		if (_conf->rtp_config.isDeveloper) {
			// Implement Later
			// loading build file
			std::ifstream in(_conf->rtp_config.build_file, std::ios::binary);
			in.read((char*)&_buildRef.version, sizeof(uint64_t));
			in.close();

			std::cout << "Build: " << _buildRef.version << std::endl;

			// Copy module_dll -> module_test_dll
			std::filesystem::path module_dll_path = _conf->rtp_config.module_dll;
			std::filesystem::path module_test_dll_path = _conf->rtp_config.module_test_dll;

			if (std::filesystem::exists(module_test_dll_path)) {
				std::filesystem::remove(module_test_dll_path);
				std::filesystem::copy_file(module_dll_path, module_test_dll_path);
			}
			else {
				std::filesystem::copy_file(module_dll_path, module_test_dll_path);
			}

			// Loading module_test_dll instead
			_library.initLibrary(_conf->rtp_config.module_test_dll);
		}
		else {
			_library.initLibrary(_conf->rtp_config.module_dll);
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

				input::doEvent(e);
			}


			if (_conf->rtp_config.isDeveloper) {
				// Periodically Check if version changes
				if (_buildRef.timer <= 0.0f) {
					uint64_t v;
					std::ifstream in(_conf->rtp_config.build_file, std::ios::binary);
					in.read((char*)&v, sizeof(uint64_t));
					in.close();

					if (v != _buildRef.version) {
						_buildRef.needsRefresh = true;
						_buildRef.version = v;
					}

					_buildRef.timer = _buildRef.maxTimer;
				}
				else {
					_buildRef.timer -= delta;
				}

				if (input::isKeyPressOnce(input::Keys::KEY_F1)) {
					_buildRef.needsRefresh = true;
					_buildRef.timer = _buildRef.maxTimer;
				}

				if (_buildRef.needsRefresh) {
					if (_library.release) {
						_library.release();
					}

					std::cout << "Build: " << _buildRef.version << std::endl;

					_library.releaseLibrary();

					// Copy module_dll -> module_test_dll
					std::filesystem::path module_dll_path = _conf->rtp_config.module_dll;
					std::filesystem::path module_test_dll_path = _conf->rtp_config.module_test_dll;

					if (std::filesystem::exists(module_test_dll_path)) {
						std::filesystem::remove(module_test_dll_path);
						std::filesystem::copy_file(module_dll_path, module_test_dll_path);
					}
					else {
						std::filesystem::copy_file(module_dll_path, module_test_dll_path);
					}

					_library.initLibrary(_conf->rtp_config.module_test_dll);


					if (_library.init) {
						_library.init(&_func_table);
					}

					_buildRef.needsRefresh = false;
				}
			}

			if (_library.update) {
				_library.update(delta);
			}

			if (_library.render) {
				_library.render();
			}

			input::update();

			SDL_GL_SwapWindow(_window);
		}
	}

	void release() {

		if (_library.release) {
			_library.release();
		}

		_library.releaseLibrary();

		if (_conf->rtp_config.isDeveloper) {
			std::filesystem::path p = _conf->rtp_config.module_test_dll;
			std::filesystem::remove(p);
		}

		ft::release(&_func_table);


		//SDL_GL_DeleteContext(_context);
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

	SDL_Window* getWindow() {
		return _window;
	}
}