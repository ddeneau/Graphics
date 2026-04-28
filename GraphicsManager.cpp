#include "GraphicsManager.h" // Link to the blueprint!

// Use GraphicsManager:: to tell the compiler this is a class method
void GraphicsManager::SwapVertexBuffer(int mode, Device device) {

    // Because we used GraphicsManager::, the compiler knows exactly
    // what vertexBuffer, flatBuffer, and device are!
    switch (mode) {
    case 0:
        // Assuming device is a pointer from GameWindow. 
        // If it isn't, change -> to .
        vertexBuffer.Bind(device->GetContext());
        break;
    case 1:
        flatBuffer.Bind(device->GetContext());
        break;
    case 2:
        heightmapBuffer.Bind(device->GetContext());
        break;
    default:
        throw std::runtime_error("Invalid render mode");
    }
}