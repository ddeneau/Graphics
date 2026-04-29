#include "GraphicsManager.h"

void GraphicsManager::LoadGraphics(Device* device) {
    Initialize(device);
    LoadShaders();
    CreateFractalResources();
}

void GraphicsManager::Initialize(Device* device) {
    m_device = device;

    m_sceneBuffer = std::make_unique<ConstantBuffer<SceneData>>(device->GetDevice());
    m_fractalBuffer = std::make_unique<ConstantBuffer<FractalData>>(device->GetDevice());
}   

void GraphicsManager::LoadShaders() {
    // We initialize our unique_ptr using make_unique.
    // This calls the Shader constructor and stores the result on the heap.
    m_shader = std::make_unique<Shader>(
        m_device->GetDevice(),
        LoadShaderFromFile("VertexShader.hlsl"),
        LoadShaderFromFile("PixelShader.hlsl")
    );
}

// 1. The High-Level
void GraphicsManager::DrawFrame(const Camera& camera, const FractalData& fd) {
    m_device->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f); // Clear canvas

    UpdateConstantBuffers(camera, fd);            // Step 1: Logic -> Math
    BindPipeline();                               // Step 2: Set GPU state
    RenderActiveMesh(fd.renderMode);              // Step 3: Draw geometry

    m_device->EndFrame();                         // Swap canvas to screen
}

// 2. Helper
void GraphicsManager::UpdateConstantBuffers(const Camera& camera, const FractalData& fd) {
    // Pack Scene (Camera) Data
    SceneData sd;
    sd.transform = DirectX::XMMatrixTranspose(camera.GetView() * camera.GetProjection());
    m_sceneBuffer->Update(m_device->GetContext(), sd);

    // Pack Fractal Data
    m_fractalBuffer->Update(m_device->GetContext(), fd);
}

// 3. Helper/Binding Step
void GraphicsManager::BindPipeline() {
    m_shader->Bind(m_device->GetContext());

    // Link constant buffers to their slots (VS Slot 0, PS Slot 0)
    m_device->GetContext()->VSSetConstantBuffers(0, 1, m_sceneBuffer->GetAddressOf());
    m_device->GetContext()->PSSetConstantBuffers(0, 1, m_fractalBuffer->GetAddressOf());
}

// 4. Rendering
void GraphicsManager::RenderActiveMesh(int mode) {
    // Choose the mesh based on the mode toggle
    VertexBuffer* target = (mode == 0) ? m_heightmapBuffer.get() : m_flatBuffer.get();

    if (target) {
        target->Bind(m_device->GetContext());
        m_device->DrawFrame(target->GetVertexCount());
    }
}

void GraphicsManager::CreateFractalResources() {
    // 1. Run the CPU Math (The Abstraction)
    Mandelbrot mandelbrot;
    auto flatVerts = GridGenerator::CreateStandardGrid(30, 30, 1);
    auto heightVerts = GridGenerator::CreateHeightmapGrid(30, 30, 1, mandelbrot);

    // 2. Upload to GPU and store in member variables
    // We use m_device->GetDevice() because we stored it in Initialize()
    m_flatBuffer = std::make_unique<VertexBuffer>(
        m_device->GetDevice(),
        flatVerts.data(),
        static_cast<int>(flatVerts.size())
    );

    m_heightmapBuffer = std::make_unique<VertexBuffer>(
        m_device->GetDevice(),
        heightVerts.data(),
        static_cast<int>(heightVerts.size())
    );
}
