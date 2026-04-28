cbuffer SceneBuffer : register(b0) {
    matrix transform;
    float time; // Make sure your C++ SceneData struct includes this float (with proper padding)
};

struct VS_Input {
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_Output {
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float3 worldPos : TEXCOORD0; // Add this!
};

VS_Output main(VS_Input input) {
    VS_Output output;

    // 1. Get the raw world position
    float4 worldPos = float4(input.pos, 1.0f);
    float uniqueOffset = input.pos.x + input.pos.z;
    float wave = sin((time * 2.0f) + uniqueOffset) * 0.5f;
    
    // Apply the wave to the Y-axis so they float up and down
    worldPos.y += wave;

    // 3. Apply the camera projection matrix
    output.pos = mul(worldPos, transform);
    
    // Pass the unique RGB grid color to the pixel shader
    output.color = input.color; 

    output.worldPos = worldPos.xyz;
    
    return output;
}