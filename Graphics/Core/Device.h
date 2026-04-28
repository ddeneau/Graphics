#pragma once
#include <d3d11.h>
#include <wrl/client.h>  // automatic COM cleanup
#include <memory>

// Wraps Direct3D 11 GPU device and context.
// Separates GPU state management from game window concerns.
class Device {
public:
    Device(HWND hwnd, int width, int height);
    ~Device() = default;

    // Access the Direct3D device and immediate context
    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetContext() const;
    IDXGISwapChain* GetSwapChain() const;
    ID3D11RenderTargetView* GetRenderTargetView() const;

    // Clear the backbuffer and set viewport
    void BeginFrame(float r, float g, float b, float a);
    
    void DrawFrame(int vertices);

    // Present the backbuffer to the screen
    void EndFrame();

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

private:
    // ComPtr automatically releases COM objects on destruction
    Microsoft::WRL::ComPtr<ID3D11Device> m_device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
    Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    int m_width;
    int m_height;
};