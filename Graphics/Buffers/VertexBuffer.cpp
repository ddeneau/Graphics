#include "../../VertexBuffer.h"
#include <stdexcept>

// Module purpose: Upload geometry data to GPU once, reuse every frame.
// Vertex buffers are the foundation of rendering; all 3D data lives here.

// Create GPU vertex buffer and upload data.
// Vertex data copied from CPU RAM to GPU VRAM; after this, GPU has instant access.
VertexBuffer::VertexBuffer(ID3D11Device* device, const Vertex* data, int vertexCount)
    : m_vertexCount(vertexCount)
{
    // Describe the buffer: how much memory, what type, how it's used.
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;           // Buffer won't change after creation (static geometry).
    bd.ByteWidth = sizeof(Vertex) * vertexCount;  // Total memory needed for all vertices.
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;  // Tell GPU this is vertex data, not texture or constant buffer.
    bd.CPUAccessFlags = 0;                    // CPU won't touch it after upload; GPU only access.

    // Package the vertex data for upload.
    D3D11_SUBRESOURCE_DATA srd = {};
    srd.pSysMem = data;  // Pointer to vertex array in CPU RAM.

    // Create the buffer on GPU and upload data in one call.
    // ComPtr automatically manages the buffer; no manual Release() needed.
    HRESULT hr = device->CreateBuffer(&bd, &srd, m_buffer.GetAddressOf());
    
    // Guard: if creation failed, throw exception; constructor will fail cleanly.
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex buffer on GPU");
    }
}

// Bind this buffer so GPU uses it for rendering.
// Before Draw() is called, GPU needs to know which vertex buffer to read from.
void VertexBuffer::Bind(ID3D11DeviceContext* context)
{
    // Tell GPU: "Use this buffer starting at offset 0, with stride = 1 vertex (32 bytes)".
    UINT stride = sizeof(Vertex);  // Size of one vertex in bytes.
    UINT offset = 0;               // Start reading from beginning of buffer.
    
    // Input Assembler stage: loads vertices from this buffer one at a time.
    context->IASetVertexBuffers(
        0,                      // Slot 0 (can have multiple buffers in slots 0-15).
        1,                      // One buffer.
        m_buffer.GetAddressOf(),  // Pointer to buffer.
        &stride,                // Bytes per vertex.
        &offset                 // Start offset.
    );

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// Return vertex count so renderer knows how many vertices to draw.
int VertexBuffer::GetVertexCount() const
{
    return m_vertexCount;
}