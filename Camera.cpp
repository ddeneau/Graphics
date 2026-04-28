#pragma once
#include <DirectXMath.h>

/**
 * Camera Class
 * Purpose: Manages the 3D "Eye" and "Lens" of the engine.
 * Logic:
 * - Position, Target, and Up are stored as XMFLOAT3 (storage types).
 * - Calculations are performed using XMMATRIX (worker types).
 * - Marked alignas(16) for SIMD compatibility.
 */
class alignas(16) Camera {
public:
    /**
     * Constructor
     * @param fovDegrees Field of View in degrees.
     * @param aspectRatio Window width / Window height.
     * @param nearZ Nearest distance visible.
     * @param farZ Furthest distance visible.
     */
    Camera(float fovDegrees, float aspectRatio, float nearZ, float farZ)
        : position(0.0f, 0.0f, -5.0f),
        target(0.0f, 0.0f, 0.0f),
        up(0.0f, 1.0f, 0.0f)
    {
        // Initialize the Projection Matrix (The Lens)
        projection = DirectX::XMMatrixPerspectiveFovLH(
            DirectX::XMConvertToRadians(fovDegrees),
            aspectRatio,
            nearZ,
            farZ
        );
    }

    /**
     * Update: Recalculates the View Matrix based on current position/target.
     */
    void Update() {
        // Create the View Matrix (The Eye)
        // Convert storage types (XMFLOAT3) to worker types (XMVECTOR) for calculation
        view = DirectX::XMMatrixLookAtLH(
            DirectX::XMLoadFloat3(&position),
            DirectX::XMLoadFloat3(&target),
            DirectX::XMLoadFloat3(&up)
        );
    }

    // Getters for the Scene manager
    DirectX::XMMATRIX GetView() const { return view; }
    DirectX::XMMATRIX GetProjection() const { return projection; }

    // Public members for easy access by InputManager/main loop
    DirectX::XMFLOAT3 position; // Camera position in world space
    DirectX::XMFLOAT3 target;   // Point camera is looking at
    DirectX::XMFLOAT3 up;       // Up direction for camera orientation
    float pitch;
    float yaw;
    float roll;

private:
    DirectX::XMMATRIX view;       // Calculated every frame
    DirectX::XMMATRIX projection; // Set once at start
};