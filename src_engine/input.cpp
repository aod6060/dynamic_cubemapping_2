#include "sys.h"


namespace input {

	// Keys
	std::vector<InputState> _keys;

	// Mouse Buttons
	std::vector<InputState> _mbs;

	// Position
	MouseCoord mousePosition;

	// Velocity
	MouseCoord mouseVelocity;

	// Mouse Wheel
	MouseWheel mouseWheel;

	// Input Mapping
	std::map<std::string, InputMap> inputMaps;

	void init() {
		_keys.resize(Keys::KEYS_MAX_SIZE);

		std::for_each(_keys.begin(), _keys.end(), [](InputState& state) {
			state = InputState::IS_RELEASE;
		});

		_mbs.resize(MouseButtons::MOUSE_MAX_SIZE);

		std::for_each(_mbs.begin(), _mbs.end(), [](InputState& state) {
			state = InputState::IS_RELEASE;
		});
	}

	void doEvent(SDL_Event& e) {
		if (e.type == SDL_KEYDOWN) {
			if (_keys[e.key.keysym.scancode] == InputState::IS_RELEASE) {
				_keys[e.key.keysym.scancode] = InputState::IS_PRESSE_ONCE;
			}
		}
		else if (e.type == SDL_KEYUP) {
			if (_keys[e.key.keysym.scancode] == InputState::IS_PRESSED) {
				_keys[e.key.keysym.scancode] = InputState::IS_RELEASE_ONCE;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN) {
			uint32_t mb = e.button.button - 1;
			if (_mbs[mb] == InputState::IS_RELEASE) {
				_mbs[mb] = InputState::IS_PRESSE_ONCE;
			}
		}
		else if (e.type == SDL_MOUSEBUTTONUP) {
			uint32_t mb = e.button.button - 1;
			if (_mbs[mb] == InputState::IS_PRESSED) {
				_mbs[mb] = InputState::IS_RELEASE_ONCE;
			}
		}
		else if (e.type == SDL_MOUSEMOTION) {
			mousePosition.x = e.motion.x;
			mousePosition.y = e.motion.y;
			mouseVelocity.x = e.motion.xrel;
			mouseVelocity.y = e.motion.yrel;
		}
		else if (e.type == SDL_MOUSEWHEEL) {
			mouseWheel.x = e.wheel.x;
			mouseWheel.y = e.wheel.y;
		}
	}

	void update() {
		std::for_each(_keys.begin(), _keys.end(), [](InputState& state) {
			if (state == InputState::IS_PRESSE_ONCE) {
				state = InputState::IS_PRESSED;
			}
			if (state == InputState::IS_RELEASE_ONCE) {
				state = InputState::IS_RELEASE;
			}
		});

		std::for_each(_mbs.begin(), _mbs.end(), [](InputState& state) {
			if (state == InputState::IS_PRESSE_ONCE) {
				state = InputState::IS_PRESSED;
			}
			if (state == InputState::IS_RELEASE_ONCE) {
				state = InputState::IS_RELEASE;
			}
		});

		mouseVelocity.x = 0;
		mouseVelocity.y = 0;

		mouseWheel.x = 0;
		mouseWheel.y = 0;
	}

	bool isKeyRelease(Keys key) {
		return _keys[key] == InputState::IS_RELEASE_ONCE || _keys[key] == InputState::IS_RELEASE;
	}

	bool isKeyPressOnce(Keys key) {
		return _keys[key] == InputState::IS_PRESSE_ONCE;
	}

	bool isKeyPress(Keys key) {
		return _keys[key] == InputState::IS_PRESSE_ONCE || _keys[key] == InputState::IS_PRESSED;
	}

	bool isKeyReleaseOnce(Keys key) {
		return _keys[key] == InputState::IS_RELEASE_ONCE;
	}


	// Mouse
	bool isMouseButtonRelease(MouseButtons mb) {
		return _mbs[mb] == InputState::IS_RELEASE_ONCE || _mbs[mb] == InputState::IS_RELEASE;
	}

	bool isMouseButtonPressOnce(MouseButtons mb) {
		return _mbs[mb] == InputState::IS_PRESSE_ONCE;
	}

	bool isMouseButtonPress(MouseButtons mb) {
		return _mbs[mb] == InputState::IS_PRESSE_ONCE || _mbs[mb] == InputState::IS_PRESSED;
	}

	bool isMouseButtonReleaseOnce(MouseButtons mb) {
		return _mbs[mb] == InputState::IS_RELEASE_ONCE;
	}

	// Mouse Coords
	void getMousePosition(MouseCoord& mc) {
		mc.x = mousePosition.x;
		mc.y = mousePosition.y;
	}

	void getMouseVelocity(MouseCoord& mc) {
		mc.x = mouseVelocity.x;
		mc.y = mouseVelocity.y;
	}

	// Mouse Wheel
	void getMouseWheel(MouseWheel& mw) {
		mw.x = mouseWheel.x;
		mw.y = mouseWheel.y;
	}

	// Grab Mode
	bool isGrab() {
		return SDL_GetRelativeMouseMode() ? true : false;
	}
	
	void setGrab(bool grab) {
		SDL_SetRelativeMouseMode((grab) ? SDL_TRUE : SDL_FALSE);
	}
	
	void toggleGrab() {
		setGrab(!isGrab());
	}

	// Input Mapping
	void createInputMapping(std::string name, const InputMap& mapping) {
		inputMaps[name] = mapping;
	}
	
	InputMap createInputMapKey(Keys key) {
		InputMap temp;
		temp.isMouseButton = false;
		temp.key = key;
		return temp;
	}
	
	InputMap createInputMapMouseButton(MouseButtons mb) {
		InputMap temp;
		temp.isMouseButton = true;
		temp.mb = mb;
		return temp;
	}
	
	void clearInputMaps() {
		inputMaps.clear();
	}
	
	bool isInputMapRelease(std::string name) {
		if (inputMaps[name].isMouseButton) {
			return isMouseButtonRelease(inputMaps[name].mb);
		}
		else {
			return isKeyRelease(inputMaps[name].key);
		}
	}
	
	bool isInputMapPressOnce(std::string name) {
		if (inputMaps[name].isMouseButton) {
			return isMouseButtonPressOnce(inputMaps[name].mb);
		}
		else {
			return isKeyPressOnce(inputMaps[name].key);
		}
	}
	
	bool isInputMapPress(std::string name) {
		if (inputMaps[name].isMouseButton) {
			return isMouseButtonPress(inputMaps[name].mb);
		}
		else {
			return isKeyPress(inputMaps[name].key);
		}
	}
	
	bool isInputMapReleaseOnce(std::string name) {
		if (inputMaps[name].isMouseButton) {
			return isMouseButtonReleaseOnce(inputMaps[name].mb);
		}
		else {
			return isKeyReleaseOnce(inputMaps[name].key);
		}
	}	

}