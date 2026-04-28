#pragma once
#include <DirectXMath.h>

// alignas(16) ensures the structure starts on a 16-byte boundary in memory.
// Constant Buffers must also be a total size that is a multiple of 16 bytes.

struct alignas(16) SceneData {
    DirectX::XMMATRIX transform; // 64 bytes - (Naturally aligned)
};

struct alignas(16) FractalData {
    // 1st 16-byte chunk
    float time;                // 4 bytes
    float zoom;                // 4 bytes
    DirectX::XMFLOAT2 offset;  // 8 bytes

    // 2nd 16-byte chunk
    int renderMode;            // 4 bytes (The toggle for Modes 1, 2, and 3)
    float padding[3];          // 12 bytes of padding to reach the next 16-byte boundary
};