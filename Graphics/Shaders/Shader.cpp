#include "Shader.h"
#include <d3dcompiler.h>
#include <stdexcept>

#pragma comment(lib, "d3dcompiler.lib")

// Module purpose: Compile HLSL shaders and manage GPU shader state.
// Shaders are programs that run on GPU; vertex shader transforms positions, pixel shader outputs colors.

// Compile vertex and pixel shaders from source strings.
Shader::Shader(ID3D11Device* device, const std::string& vertexSource, const std::string& pixelSource)
{
    Microsoft::WRL::ComPtr<ID3DBlob> vsBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> psBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    // Compile vertex shader from source string.
    HRESULT hr = D3DCompile(
        vertexSource.c_str(),           // Source code
        vertexSource.size(),            // Length
        "VertexShader",                 // Name (for debug)
        nullptr,                        // Macros
        nullptr,                        // Includes
        "main",                         // Entry point function
        "vs_5_0",                       // Target profile (vertex shader, DirectX 11)
        0,                              // Compile flags
        0,                              // Effect flags
        vsBlob.GetAddressOf(),          // Output bytecode
        errorBlob.GetAddressOf()        // Error messages
    );

    // Guard: if compilation failed, throw exception with error message.
    if (FAILED(hr)) {
        if (errorBlob) {
            throw std::runtime_error("Vertex shader compile error: " + 
                std::string((char*)errorBlob->GetBufferPointer()));
        }
        throw std::runtime_error("Vertex shader compilation failed");
    }

    // Compile pixel shader from source string.
    errorBlob.Reset();
    hr = D3DCompile(
        pixelSource.c_str(),
        pixelSource.size(),
        "PixelShader",
        nullptr, nullptr,
        "main",
        "ps_5_0",                       // Pixel shader, DirectX 11
        0, 0,
        psBlob.GetAddressOf(),
        errorBlob.GetAddressOf()
    );

    if (FAILED(hr)) {
        if (errorBlob) {
            throw std::runtime_error("Pixel shader compile error: " + 
                std::string((char*)errorBlob->GetBufferPointer()));
        }
        throw std::runtime_error("Pixel shader compilation failed");
    }

    // Create GPU vertex shader from bytecode.
    hr = device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), 
                                     nullptr, m_vertexShader.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create vertex shader");
    }

    // Create GPU pixel shader from bytecode.
    hr = device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), 
                                    nullptr, m_pixelShader.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create pixel shader");
    }

    // Create input layout: tells GPU how to interpret vertex data.
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "ORIGN", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), 
                                    vsBlob->GetBufferSize(), m_inputLayout.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create input layout");
    }
}

// Bind shaders to GPU so they execute during Draw().
void Shader::Bind(ID3D11DeviceContext* context)
{
    // Tell GPU which vertex shader to use.
    context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

    // Tell GPU which pixel shader to use.
    context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

    // Tell GPU how to interpret vertex buffer data.
    context->IASetInputLayout(m_inputLayout.Get());
}