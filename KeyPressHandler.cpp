// Handles all key presses and their effects on the program (needs to be taken out of main and abstracted in)
#include "KeyPressHandler.h"
#include <iostream>
#include <DirectXMath.h>

using namespace DirectX;

void KeyPressHandler::ApplyFPSMovement(InputManager* inputManager, Camera& camera, float speed) {
    auto key = inputManager->GetAndConsumeKey();
    if (!key) return;

    XMFLOAT3 currentPos = camera.GetPosition();
    XMFLOAT3 currentTarget = camera.GetTarget();
    XMFLOAT3 currentUp = camera.GetUp();

    XMVECTOR posVec = XMLoadFloat3(&currentPos);
    XMVECTOR targetVec = XMLoadFloat3(&currentTarget);
    XMVECTOR upVec = XMLoadFloat3(&currentUp);

    XMVECTOR forward = XMVector3Normalize(XMVectorSubtract(targetVec, posVec));
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(upVec, forward));

    char k = *key;
    XMVECTOR movement = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

    if (k == 'w' || k == 'W') movement = XMVectorScale(forward, speed);
    if (k == 's' || k == 'S') movement = XMVectorScale(forward, -speed);
    if (k == 'a' || k == 'A') movement = XMVectorScale(right, -speed);
    if (k == 'd' || k == 'D') movement = XMVectorScale(right, speed);

    posVec = XMVectorAdd(posVec, movement);
    targetVec = XMVectorAdd(targetVec, movement);

    XMStoreFloat3(&currentPos, posVec);
    XMStoreFloat3(&currentTarget, targetVec);

    camera.SetPosition(currentPos);
    camera.SetTarget(currentTarget);
}

int KeyPressHandler::HandleKeyPress(InputManager* inputManager, Camera camera, DirectX::XMFLOAT2 offset, float zoom, float speed, int currentMode) {
    auto key = "";
    int mode = currentMode;

    if (key) {
        // Handle Movement logic
        ApplyFPSMovement(inputManager, camera, speed);

        // Handle Mode switches (don't consume here if RegisterEngineCommands needs them)
        if (*key == '1') mode = 1;
        if (*key == '2') mode = 2;
    }

    // Crucial: Update the view matrix after moving!
    camera.Update();

    return mode;
}

void KeyPressHandler::RegisterEngineCommands(CommandHandler& commands, bool& running, InputManager* input_manager) {
    auto quit = [&running]() { running = false; };
    auto show_history = [input_manager]() {
        const auto& h = input_manager->GetKeyHistory();
        if (h.is_empty()) return;
        std::cout << "Key history: ";
        for (char k : h.get_history()) std::cout << k;
        std::cout << '\n';
        };

    commands.register_command('q', quit);
    commands.register_command('Q', quit);
    commands.register_command('c', [input_manager]() {
        input_manager->ClearHistory();
        std::cout << "History cleared.\n";
        });
    commands.register_command('C', [input_manager]() {
        input_manager->ClearHistory();
        std::cout << "History cleared.\n";
        });
    commands.register_command('\t', show_history);
}

