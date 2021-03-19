#include "sys.h"


namespace ft {


	void init(Table* table) {
		// App
		table->app.getWidth = app::getWidth;
		table->app.getHeight = app::getHeight;
		table->app.exit = app::exit;
		table->app.getWindow = app::getWindow;
		
		// Input
		
		// Keyboard
		table->input.isKeyRelease = input::isKeyRelease;
		table->input.isKeyPressOnce = input::isKeyPressOnce;
		table->input.isKeyPress = input::isKeyPress;
		table->input.isKeyReleaseOnce = input::isKeyReleaseOnce;
		
		// Mouse Button
		table->input.isMouseButtonRelease = input::isMouseButtonRelease;
		table->input.isMouseButtonPressOnce = input::isMouseButtonPressOnce;
		table->input.isMouseButtonPress = input::isMouseButtonPress;
		table->input.isMouseButtonReleaseOnce = input::isMouseButtonReleaseOnce;
		
		// Mouse Coord
		table->input.getMousePosition = input::getMousePosition;
		table->input.getMouseVelocity = input::getMouseVelocity;
		
		// Mouse Wheel
		table->input.getMouseWheel = input::getMouseWheel;

		// Grab
		table->input.isGrab = input::isGrab;
		table->input.setGrab = input::setGrab;
		table->input.toggleGrab = input::toggleGrab;

		// Input Map
		table->input.createInputMapping = input::createInputMapping;
		table->input.createInputMapKey = input::createInputMapKey;
		table->input.createInputMapMouseButton = input::createInputMapMouseButton;
		table->input.clearInputMaps = input::clearInputMaps;
		table->input.isInputMapRelease = input::isInputMapRelease;
		table->input.isInputMapPressOnce = input::isInputMapPressOnce;
		table->input.isInputMapPress = input::isInputMapPress;
		table->input.isInputMapReleaseOnce = input::isInputMapReleaseOnce;

	}

	void release(Table* table) {
		// App
		table->app.getWidth = nullptr;
		table->app.getHeight = nullptr;
		table->app.exit = nullptr;
		// Input
		table->input.isKeyRelease = nullptr;
		table->input.isKeyPressOnce = nullptr;
		table->input.isKeyPress = nullptr;
		table->input.isKeyReleaseOnce = nullptr;
		// Mouse Button
		table->input.isMouseButtonRelease = nullptr;
		table->input.isMouseButtonPressOnce = nullptr;
		table->input.isMouseButtonPress = nullptr;
		table->input.isMouseButtonReleaseOnce = nullptr;
		// Mouse Coord
		table->input.getMousePosition = nullptr;
		table->input.getMouseVelocity = nullptr;
		// Mouse Wheel
		table->input.getMouseWheel = nullptr;
		// Grab
		table->input.isGrab = nullptr;
		table->input.setGrab = nullptr;
		table->input.toggleGrab = nullptr;
		// Input Map
		table->input.createInputMapping = nullptr;
		table->input.createInputMapKey = nullptr;
		table->input.createInputMapMouseButton = nullptr;
		table->input.clearInputMaps = nullptr;
		table->input.isInputMapRelease = nullptr;
		table->input.isInputMapPressOnce = nullptr;
		table->input.isInputMapPress = nullptr;
		table->input.isInputMapReleaseOnce = nullptr;
	}
}