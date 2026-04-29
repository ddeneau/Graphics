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
    Camera(float fovDegrees, float aspectRatio, float nearZ, float farZ);

    /**
     * Update: Recalculates the View Matrix based on current position/target.
     */
    void Update();

    // Getters for the Scene manager
    DirectX::XMMATRIX GetView() const;
    DirectX::XMMATRIX GetProjection() const;
	DirectX::XMFLOAT2 GetOffset() const { return offset; }

    // Public members for easy access by InputManager/main loop
    DirectX::XMFLOAT3 position; // Camera position in world space
    DirectX::XMFLOAT3 target;   // Point camera is looking at
    DirectX::XMFLOAT3 up;       // Up direction for camera orientation
    DirectX::XMFLOAT2 offset;

    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

private:
    DirectX::XMMATRIX view;       // Calculated every frame
    DirectX::XMMATRIX projection; // Set once at start
};