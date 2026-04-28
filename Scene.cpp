#include<directxmath.h>

struct alignas(16) SceneData {
    DirectX::XMMATRIX transform;
};

struct alignas(16) FractalData {
    float time;
    float zoom;
    DirectX::XMFLOAT2 offset;
};