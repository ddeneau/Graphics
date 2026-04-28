#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>

// Wraps compiled vertex and pixel shaders.
// Compiles HLSL source at runtime; binds to GPU pipeline before drawing.
class Shader {
public:
    // Compile vertex and pixel shaders from source strings.
    // Throws exception if compilation fails.
    Shader(ID3D11Device* device, const std::string& vertexSource, const std::string& pixelSource);
    
    // Destructor automatically releases shaders via ComPtr.
    ~Shader() = default;

    // Bind shaders to GPU pipeline so Draw() uses them.
    void Bind(ID3D11DeviceContext* context);

private:
    // ComPtr automatically releases COM objects on destruction.
    Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};