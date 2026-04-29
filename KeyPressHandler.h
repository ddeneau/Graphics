#pragma once
#include "InputManager.h"
#include <DirectXMath.h>
#include "Camera.h"
#include "CommandHandler.h"	

class KeyPressHandler {

public:
	InputManager inputManager;

	int HandleKeyPress(InputManager* inputManager, Camera camera, DirectX::XMFLOAT2 offset, float speed, float zoom, int currentMode);
	void RegisterEngineCommands(CommandHandler& commands, bool& running, InputManager* input_manager);

private:

};
