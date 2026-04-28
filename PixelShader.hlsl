// Project1/PixelShader.hlsl
cbuffer FractalBuffer : register(b0) {
    float time;
    float zoom;
    float2 offset;
    int renderMode;
    float3 padding; // byte alingment
};

struct PS_Input {
    float4 pos : SV_POSITION; // These are screen-space coordinates
    float4 color : COLOR;
};

float4 main(PS_Input input) : SV_TARGET {
    if (renderMode == 0) {
        return input.color; // Render mode 0 is just colors.
    }

    // Render mode 1 is the first thing I had.
    if (renderMode == 1) {
        // Standardize the mapping
        float2 uv = input.pos.xy / float2(800, 600);
        float2 c = (uv - 0.5) * zoom + offset;
    
        float2 z = float2(0, 0);
        int iter = 0;
        const int max_iter = 100;

        for (iter = 0; iter < max_iter; iter++) {
            float x = z.x * z.x - z.y * z.y + c.x;
            float y = 2.0 * z.x * z.y + c.y;
            if (x*x + y*y > 4.0) break;
            z = float2(x, y);
        }

        // Change: Don't return pure black for the background during testing
        float t = (float)iter / (float)max_iter;
        if (iter == max_iter) return float4(0.1f, 0.1f, 0.1f, 1.0f); // Dark grey center

        return float4(
            0.5 + 0.5 * sin(3.0 * t + time), 
            0.5 + 0.5 * sin(5.0 * t + time * 1.1), 
            0.5 + 0.5 * sin(7.0 * t + time * 1.2), 
            1.0f
        );
    }

    // No uvs - use world coords. Maybe if we take it out of the statement it will persist.
    if (renderMode == 2) {
        float2 z, c;
        c.x = (input.worldPos.x * zoom) + offset.x;
        c.y = (input.worldPos.z * zoom) + offset.y;
        z = c;

        int iter = 0;
        int maxIter = 1000;
        while(iter < maxIter && (z.x*z.x + z.y*z.y) < 4.0) {
            float x_new = z.x*z.x - z.y*z.y + c.x;
            z.y = 2.0 * z.x * z.y + c.y;
            z.x = x_new;
            iter++;
        }

        float fractalValue = (float)iter / (float)maxIter;
        float4 fractalColor = float4(fractalValue, fractalValue * 0.5f, sin(fractalValue * 3.1415f + time), 1.0f); // idk PI seemed right..

    }

    if (renderMode == 4) {
        return fractalColor; // world coordiante set.
    }

    if (renderMode == 5) {
        return input.color * fractalColor;
    }
    
    return input.color;
}