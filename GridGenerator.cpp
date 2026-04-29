
#include <vector>
#include "VertexBuffer.h" // Assuming this holds your Vertex struct
#include "Mandelbrot.h" // Your mandelbrot class
#include "GridGenerator.h"


std::vector<Vertex> GridGenerator::CreateStandardGrid(int width, int height, int depth) {
    std::vector<Vertex> vertices;
    float spacing = 1.2f;
    float size = 0.1f;
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float cx = (x - (width / 2.0f)) * spacing;
                float cy = (y - (height / 2.0f)) * spacing;
                float cz = z * spacing;
                float r = (float)x / width;
                float g = (float)y / height;
                float b = (float)z / depth;
                // Triangle 1
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx - size, cy - size, cz,  r, g, b, 1.0f });
                // Triangle 2
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
            }
        }
    }
    return vertices;
}

std::vector<Vertex> GridGenerator::CreateHeightmapGrid(int width, int height, int depth, Mandelbrot& mandelbrot) {
    std::vector<Vertex> vertices;
    float spacing = 1.2f;
    float size = 0.1f;

    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float cx = (x - (width / 2.0f)) * spacing;
                float cy = (y - (height / 2.0f)) * spacing;

                // OPTION A LOGIC: Use the Mandelbrot to calculate physical extrusion!
                // We scale cx and cy down slightly so the fractal fits on the grid
                int iter = mandelbrot.MandelbrotIteration(cx * 0.1, cy * 0.1, 100);

                // Extrude the depth (Z) based on how many iterations it took
                // High iterations = pushed further towards the camera
                float extrusion = (iter / 100.0f) * 15.0f;
                float cz = (z * spacing) - extrusion;

                float r = (float)x / width;
                float g = (float)y / height;
                float b = (float)z / depth;

                // Triangle 1
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx - size, cy - size, cz,  r, g, b, 1.0f });
                // Triangle 2
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
            }
        }
    }
    return vertices;
}

std::vector<Vertex> GridGenerator::CreateFractalGrid(int width, int height, int depth, Mandelbrot& mandelbrot, float zoom, float offset) {
    std::vector<Vertex> vertices;
    float spacing = 1.2f;
    float size = 0.1f;
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float cx = (x - (width / 2.0f)) * spacing;
                float cy = (y - (height / 2.0f)) * spacing;
                // OPTION C LOGIC: Use the Mandelbrot to calculate physical extrusion!
                // We scale cx and cy down slightly so the fractal fits on the grid
                int iter = mandelbrot.MandelbrotIteration((cx * zoom) + offset, (cy * zoom) + offset, 100);
                // Extrude the depth (Z) based on how many iterations it took
                // High iterations = pushed further towards the camera
                float extrusion = (iter / 100.0f) * 15.0f;
                float cz = (z * spacing) - extrusion;
                float r = (float)x / width;
                float g = (float)y / height;
                float b = (float)z / depth;
                // Triangle 1
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx - size, cy - size, cz,  r, g, b, 1.0f });
                // Triangle 2
                vertices.push_back({ cx - size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy + size, cz,  r, g, b, 1.0f });
                vertices.push_back({ cx + size, cy - size, cz,  r, g, b, 1.0f });
            }
        }
    }
    return vertices;
}

