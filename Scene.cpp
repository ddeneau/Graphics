#include "Scene.h"
#include "Node.h"
#include "GraphicsManager.h"

Scene::Scene(GraphicsManager* gfx, float fov, float aspect, float nearZ, float farZ)
    : m_gfx(gfx)
{

    m_camera = std::make_unique<Camera>(fov, aspect, nearZ, farZ);

    m_rootNode = std::make_shared<Node>();

    m_globalFractalData = {};
    m_globalFractalData.zoom = 1.0f;
    m_globalFractalData.renderMode = 0;
}

void Scene::Update(float deltaTime, float totalTime) {
    m_camera->Update();
    m_globalFractalData.time = totalTime;
}

void Scene::SetMode(int mode) {
    m_globalFractalData.renderMode = mode;
}

void Scene::Render() {
    m_gfx->DrawFrame(*m_camera, m_globalFractalData);
}