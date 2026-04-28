// Main entry point for the application; initializes window, graphics device, and game loop, then starts the main loop.
#define NOMINMAX
#include <iostream>
#include "GameLoop.h"
#include "GameWindow.h"
#include "Graphics/Core/Device.h"
#include "Graphics/Shaders/Shader.h"
#include "Graphics/Shaders/ShaderLoader.h"
#include "VertexBuffer.h"
#include "InputManager.h"
#include "KeyHistory.h"
#include "CommandHandler.h"
#include "ConstantBuffer.h"
#include "Scene.h"
#include "Camera.h"
#include "Mandelbrot.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib") 

static void RegisterEngineCommands(CommandHandler& commands, bool& running, InputManager* input_manager) {
    auto quit = [&running]() { running = false; };
    auto show_history = [input_manager]() {
        const auto& h = input_manager->GetKeyHistory();
        if (h.is_empty()) return;
        std::cout << "Key history: ";
        for (char k : h.get_history()) std::cout << k;
        std::cout << '\n';
        };

    commands.register_command('q', quit);
    commands.register_command('Q', quit);
    commands.register_command('c', [input_manager]() {
        input_manager->ClearHistory();
        std::cout << "History cleared.\n";
        });
    commands.register_command('C', [input_manager]() {
        input_manager->ClearHistory();
        std::cout << "History cleared.\n";
        });
    commands.register_command('\t', show_history);
}



int main() {

    try {
        GameWindow window(GetModuleHandle(nullptr), 800, 600, "Game Engine");
        auto input_manager = window.GetInputManager();
        auto device = window.GetGraphicsDevice();
        Camera camera(45.0f, 400.0f / 300.0f, 0.1f, 100.0f);
        ConstantBuffer<SceneData> constantBuffer(device->GetDevice());
        ConstantBuffer<FractalData> fractalBuffer(device->GetDevice());
        Mandelbrot mandelbrot = Mandelbrot();
        float currentZoom = 2.0f; //
        DirectX::XMFLOAT2 currentOffset = { -0.5f, 0.0f }; //
        const float moveSpeed = 0.02f; //
        CommandHandler commands;
        GameLoop game_loop(60);
        bool running = true;

        camera.position = DirectX::XMFLOAT3(5.0f, 5.0f, -5.0f);  //
        camera.target = DirectX::XMFLOAT3(0.0f, 0.0f, 15.0f);  //
        camera.up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); //

        RegisterEngineCommands(commands, running, input_manager);
     
        Shader shader(device->GetDevice(),
            LoadShaderFromFile("VertexShader.hlsl"),
            LoadShaderFromFile("PixelShader.hlsl"));


        while (running && window.IsOpen()) {
            if (!window.ProcessMessages()) break;

            game_loop.update([&]() {

                // Controls Start
				// Check for input and execute commands (not defined yet?); if no command, print the key
                auto key = input_manager->GetAndConsumeKey();
                if (key) {
                    if (*key == 'w' || *key == 'W') currentOffset.y -= moveSpeed * currentZoom;
                    if (*key == 's' || *key == 'S') currentOffset.y += moveSpeed * currentZoom;
                    if (*key == 'a' || *key == 'A') currentOffset.x -= moveSpeed * currentZoom;
                    if (*key == 'd' || *key == 'D') currentOffset.x += moveSpeed * currentZoom;

                    // Zooming controls
                    if (*key == 'z' || *key == 'Z') currentZoom *= 0.95f; // Zoom In
                    if (*key == 'x' || *key == 'X') currentZoom *= 1.05f; // Zoom Out
                }
                


                device->BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

                camera.Update();
                SceneData sd;
                sd.transform = DirectX::XMMatrixTranspose(camera.GetView() * camera.GetProjection());
                constantBuffer.Update(device->GetContext(), sd);
                device->GetContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf()); // Bind to VS Slot 0
                device->EndFrame();
                });
        }
    }
    catch (const std::exception& e) {
       
        std::cerr << "\n[FATAL ENGINE ERROR]" << e.what() << "\n\n";
        system("pause");
        return -1;
    }

    return 0;
}