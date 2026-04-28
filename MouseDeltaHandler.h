#pragma once
#include "Camera.h"
#include  "InputManager.h"

class MouseDeltaHandler {

public:

	MouseDeltaHandler() = default;
	MouseDelta mouseDelta;
	float x = 0;
	float y = 0;
	bool rmbDown = false;
	bool lmbDown = false;
	bool mmbDown = false;
	bool mb1Down = false; // likely unused
	bool mb2Down = false; // likely unused

	void HandleMouseDelta(Camera& camera, InputManager* inputManager);

private:

};