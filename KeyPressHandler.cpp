// Handles all key presses and their effects on the program (needs to be taken out of main and abstracted in)
#include "KeyPressHandler.h"

void KeyPressHandler::HandleKeyPress(InputManager* inputManager, DirectX::XMFLOAT2 offset, float zoom, float speed) {
    

    auto key = inputManager->GetAndConsumeKey();
    if (key) {
        if (*key == 'w' || *key == 'W') offset.y -= speed * zoom;
        if (*key == 's' || *key == 'S') offset.y += speed * zoom;
        if (*key == 'a' || *key == 'A') offset.x -= speed * zoom;
        if (*key == 'd' || *key == 'D') offset.x += speed * zoom;

        // Zooming controls
        if (*key == 'z' || *key == 'Z') zoom *= 0.95f; // Zoom In
        if (*key == 'x' || *key == 'X') zoom *= 1.05f; // Zoom Out
    }
}
