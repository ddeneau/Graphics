#pragma once
#include <string>

// Load shader source code from file as string.
// Throws exception if file not found or read fails.
std::string LoadShaderFromFile(const std::string& filePath);