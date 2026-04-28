// Handles mouse actions and their effects on the program (needs to be taken out of main and abstracted in)
#define NOMINMAX
#include "MouseDeltaHandler.h"
#include <algorithm>

void MouseDeltaHandler::HandleMouseDelta(Camera& camera, InputManager* inputManager) {

    // We use the variables defined in the header blueprint
    mouseDelta = inputManager->GetMouseDelta();
    
    if (inputManager->IsRightMouseDown()) { // Add parentheses to fix syntax error
        const float sensitivity = 0.005f;
        camera.yaw += mouseDelta.x * sensitivity;
        camera.pitch -= mouseDelta.y * sensitivity;
        // Clamp pitch to avoid gimbal lock
        camera.pitch = std::max(-1.55f, std::min(1.55f, camera.pitch));
        // Calculate forward vector based on updated yaw and pitch
        DirectX::XMVECTOR forward = DirectX::XMVectorSet(
            cos(camera.pitch) * sin(camera.yaw),
            sin(camera.pitch),
            cos(camera.pitch) * cos(camera.yaw),
            0.0f
        );
        // Update camera target based on new forward vector
        DirectX::XMVECTOR camPos = DirectX::XMLoadFloat3(&camera.position);
        DirectX::XMVECTOR newTarget = DirectX::XMVectorAdd(camPos, forward);
        DirectX::XMStoreFloat3(&camera.target, newTarget);
    }

}