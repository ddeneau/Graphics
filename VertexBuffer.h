#pragma once
#include <d3d11.h>
#include <wrl/client.h>

// Simple vertex structure: position and color.
// Each vertex is 32 bytes (8 floats × 4 bytes each).
struct Vertex {
    float x, y, z;              // Position in 3D space
    float r, g, b, a;           // Color (RGBA)
	float origin_x, origin_y; // Original position for fractal calculations
};

// Wraps Direct3D vertex buffer on GPU.
// Manages GPU memory for vertex data; one upload, reuse every frame.
class VertexBuffer {
public:
    // Create vertex buffer and upload data to GPU VRAM.
    // Throws exception if buffer creation fails.
    VertexBuffer(ID3D11Device* device, const Vertex* data, int vertexCount);
    
    // Destructor automatically releases GPU buffer via ComPtr.
    ~VertexBuffer() = default;

    // Tell GPU context to use this buffer for rendering.
    // Must be called before Draw().
    void Bind(ID3D11DeviceContext* context);
    
    // Get number of vertices in this buffer.
    int GetVertexCount() const;

    // Prevent copying; GPU resource can't be safely duplicated.
    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

private:
    // ComPtr automatically releases COM objects on destruction (RAII for GPU memory).
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
    
    int m_vertexCount;  // How many vertices in this buffer.
};