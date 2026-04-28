#include "Device.h"
#include <d3d11.h>
#include <stdexcept>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

// Initialize Direct3D device, context, swap chain, and render target view.
Device::Device(HWND hwnd, int width, int height)
    : m_width(width), m_height(height)
{
   
    D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    // Create device and immediate context; this is the GPU connection
    HRESULT hr = D3D11CreateDevice(
        nullptr,                           // Default adapter
        D3D_DRIVER_TYPE_HARDWARE,          // Use GPU
        nullptr,                           // No software rasterizer
        D3D11_CREATE_DEVICE_DEBUG,         // 0 or D3D11_CREATE_DEVICE_DEBUG
        featureLevels, 1,                  // Feature levels to request
        D3D11_SDK_VERSION,
        &m_device,                         // Output device
        nullptr,                           // Output feature level (optional)
        &m_context                         // Output context
    );

    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create Direct3D device");
    }

    // Get the underlying DXGI factory to create swap chain
    Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
    m_device.As(&dxgiDevice);

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    dxgiDevice->GetAdapter(&adapter);

    Microsoft::WRL::ComPtr<IDXGIFactory> factory;
    adapter->GetParent(IID_PPV_ARGS(&factory));

    // Describe the swap chain: double-buffered, RGBA
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = TRUE;

    // Create swap chain; this manages backbuffer and presentation
    hr = factory->CreateSwapChain(m_device.Get(), &scd, &m_swapChain);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create swap chain");
    }

    // Get the backbuffer texture
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

    // Create render target view: tells GPU where to draw
    m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
}

ID3D11Device* Device::GetDevice() const
{
    return m_device.Get();
}

ID3D11DeviceContext* Device::GetContext() const
{
    return m_context.Get();
}

IDXGISwapChain* Device::GetSwapChain() const
{
    return m_swapChain.Get();
}

ID3D11RenderTargetView* Device::GetRenderTargetView() const
{
    return m_renderTargetView.Get();
}

// Clear backbuffer to solid color and set viewport for rendering
void Device::BeginFrame(float r, float g, float b, float a)
{
    float clearColor[] = { r, g, b, a };
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

    // Set viewport: where on screen to render
    D3D11_VIEWPORT vp = {};
    vp.Width = static_cast<float>(m_width);
    vp.Height = static_cast<float>(m_height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_context->RSSetViewports(1, &vp);

    // Set render target
    m_context->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);
}

void Device::DrawFrame(int vertices)
{
    m_context->Draw(vertices, 0);
}

// Show the backbuffer on screen
void Device::EndFrame()
{
    m_swapChain->Present(1, 0);  // Vsync on (1 = wait for vertical blank)
}