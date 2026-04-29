#include "Camera.h"

Camera::Camera(float fovDegrees, float aspectRatio, float nearZ, float farZ)
    : position(0.0f, 0.0f, -5.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
	offset(0.0f, 0.0f),
    view(DirectX::XMMatrixIdentity()) // Initialize view to identity matrix (MIGHT GIVE A WEIRD VIEW)
{
    // Initialize the Projection Matrix (The Lens)
    projection = DirectX::XMMatrixPerspectiveFovLH(
        DirectX::XMConvertToRadians(fovDegrees),
        aspectRatio,
        nearZ,
        farZ
    );
}

void Camera::Update() {
    // Create the View Matrix (The Eye)
    // Convert storage types (XMFLOAT3) to worker types (XMVECTOR) for calculation
    view = DirectX::XMMatrixLookAtLH(
        DirectX::XMLoadFloat3(&position),
        DirectX::XMLoadFloat3(&target),
        DirectX::XMLoadFloat3(&up)
    );
}

DirectX::XMFLOAT3 Camera::GetTarget() {
	return target;
}

DirectX::XMFLOAT3 Camera::GetPosition() {
    return position;
}

DirectX::XMFLOAT3 Camera::GetUp() {
    return up;
}

DirectX::XMMATRIX Camera::GetView() const {
    return view;
}

DirectX::XMMATRIX Camera::GetProjection() const {
    return projection;
}