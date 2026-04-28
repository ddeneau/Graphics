#include "ShaderLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

// Module purpose: Load shader files from disk into strings.
// Separates shader code from C++; shaders stored in separate .hlsl files.

// Read shader file and return contents as string.
std::string LoadShaderFromFile(const std::string& filePath)
{
    // Open file for reading.
    std::ifstream file(filePath);
    
    // Guard: if file doesn't exist or can't open, throw exception.
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open shader file: " + filePath);
    }

    // Read entire file into string stream.
    std::stringstream buffer;
    buffer << file.rdbuf();
    
    // Return file contents as string.
    return buffer.str();
}