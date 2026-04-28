#pragma once
#include <vector>
#include "VertexBuffer.h"
#include "Mandelbrot.h"

class GridFactory {
public:
    // Generates the standard flat grid for Options B and C
    static std::vector<Vertex> CreateStandardGrid(int width, int height, int depth);

    // Generates the physically deformed grid for Option A
    static std::vector<Vertex> CreateHeightmapGrid(int width, int height, int depth, Mandelbrot& mandelbrot);

	static std::vector<Vertex> CreateFractalGrid(int width, int height, int depth, Mandelbrot& mandelbrot, float zoom, float offset);
};