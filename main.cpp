#include "GameLoop.h"
#include "GameWindow.h"
#include "GraphicsManager.h"
#include "MouseDeltaHandler.h"
#include "KeyPressHandler.h"
#include "Scene.h"
#include "GridGenerator.h"
#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

int main() {
    try {
        GameWindow window(GetModuleHandle(nullptr), 800, 600, "Fractal Engine v2");
        auto device = window.GetGraphicsDevice();
        auto input = window.GetInputManager();
        GraphicsManager gfx;
        Scene gameScene(&gfx, 45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
        Camera* cam = gameScene.GetCamera();
        MouseDeltaHandler mouseHandler;
        KeyPressHandler keyPressHandler;
        bool running = true;
        GameLoop game_loop(60);
        float total_time = 0.0f;

        cam->position = { 5.0f, 5.0f, -5.0f };
        cam->target = { 0.0f, 0.0f, 15.0f};

        gfx.LoadGraphics(device);

        while (running && window.IsOpen()) {
            window.ProcessMessages();
            game_loop.update([&]() {
                mouseHandler.HandleMouseDelta(*gameScene.GetCamera(), input);
				int mode = keyPressHandler.HandleKeyPress(input, *gameScene.GetCamera(), cam->GetOffset(), 5.0f, 1.0f, 0);
				gameScene.SetMode(mode);
                gameScene.Update(game_loop.get_delta_time(), total_time);
                gameScene.Render();
				total_time += game_loop.get_delta_time();
                });
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\n[FATAL ERROR] " << e.what() << "\n";
        system("pause");
        return -1;
    }
    return 0;
}