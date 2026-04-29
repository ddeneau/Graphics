#pragma once
#include <memory>
#include <DirectXMath.h>     
#include "Camera.h"          

class GraphicsManager;
class Node;


struct alignas(16) SceneData {
    DirectX::XMMATRIX transform;
};

struct alignas(16) FractalData {
    float time;
    float zoom;
    DirectX::XMFLOAT2 offset;
    int renderMode;
    float padding[3];
};

class Scene {
public:
    Scene(GraphicsManager* gfx, float fov, float aspect, float nearZ, float farZ);

    void Update(float deltaTime, float totalTime);
    void Render();
    void SetMode(int mode);

    std::shared_ptr<Node> GetRoot() { return m_rootNode; }
    Camera* GetCamera() { return m_camera.get(); }

private:
    GraphicsManager* m_gfx;
    std::unique_ptr<Camera> m_camera;
    std::shared_ptr<Node> m_rootNode;
    FractalData m_globalFractalData;
};