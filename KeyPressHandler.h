#pragma once
#include "InputManager.h"
#include <DirectXMath.h>

class KeyPressHandler {

public:
	InputManager inputManager;

	void HandleKeyPress(InputManager* inputManager, DirectX::XMFLOAT2 offset, float speed, float zoom);

private:

};
