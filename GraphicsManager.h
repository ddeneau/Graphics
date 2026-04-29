#pragma once
#include <memory>         // Required for unique_ptr
#include <DirectXMath.h>  // Required for XMMATRIX
#include "Graphics/Core/Device.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Shaders/ShaderLoader.h" // For LoadShaderFromFile
#include "GridGenerator.h"
#include <iostream>
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Scene.h"        // For SceneData and FractalData
#include "Camera.h"

class GraphicsManager {
public:
    GraphicsManager() = default;

    void LoadGraphics(Device* device);
    void Initialize(Device* device);
    void LoadShaders();

    // This signature MUST match the .cpp exactly
    void DrawFrame(const Camera& camera, const FractalData& fd);

private:
    // These helpers MUST be declared here to be used in the .cpp
    void UpdateConstantBuffers(const Camera& camera, const FractalData& fd);
    void BindPipeline();
    void RenderActiveMesh(int mode);
    void CreateFractalResources();
    // Data Members
    Device* m_device = nullptr;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_flatBuffer;
    std::unique_ptr<VertexBuffer> m_heightmapBuffer;

    // Use unique_ptr for these to avoid the "Deleted Constructor" error
    std::unique_ptr<ConstantBuffer<SceneData>> m_sceneBuffer;
    std::unique_ptr<ConstantBuffer<FractalData>> m_fractalBuffer;
}; // <--- MAKE SURE THIS SEMICOLON IS HERE!