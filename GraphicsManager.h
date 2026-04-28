#pragma once
#include <vector>
#include "ConstantBuffer.h"
#include "Graphics/Core/Device.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Shaders/ShaderLoader.h"
#include "VertexBuffer.h" 
#include "Mandelbrot.h" // CRITICAL: Only include .h files!
#include "Scene.h"      // CRITICAL: Only include .h files!

// Handles rendering pipelines
class GraphicsManager {

public:


	Device* device; 
	std::unique_ptr<Shader> shader;
	VertexBuffer vertexBuffer;
	VertexBuffer flatBuffer;
	VertexBuffer heightmapBuffer;
	ConstantBuffer<SceneData> sceneBuffer;
	ConstantBuffer<FractalData> fractalBuffer;
	FractalData fractalData;
	int renderMode;

	void LoadShaders();
	void SwapVertexBuffer(int mode);
	void BindShadersAndBuffers();
	void DrawFrame(int mode);
};