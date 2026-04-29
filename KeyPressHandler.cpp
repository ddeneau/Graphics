// Handles all key presses and their effects on the program (needs to be taken out of main and abstracted in)
#include "KeyPressHandler.h"
#include <iostream>

int KeyPressHandler::HandleKeyPress(InputManager* inputManager, Camera camera, DirectX::XMFLOAT2 offset, float zoom, float speed, int currentMode) {
    auto key = inputManager->GetAndConsumeKey();
    int mode = currentMode;

    if (key) {
        if (*key == 'w' || *key == 'W') offset.y -= speed * zoom;
        if (*key == 's' || *key == 'S') offset.y += speed * zoom;
        if (*key == 'a' || *key == 'A') offset.x -= speed * zoom;
        if (*key == 'd' || *key == 'D') offset.x += speed * zoom;

        // Zooming controls
        if (*key == 'z' || *key == 'Z') zoom *= 0.95f; // Zoom In
        if (*key == 'x' || *key == 'X') zoom *= 1.05f; // Zoom Out

        if (key == '1') mode = 1;
        if (key == '2') mode = 2;
    }
    return mode;
}

void KeyPressHandler::RegisterEngineCommands(CommandHandler& commands, bool& running, InputManager* input_manager) {
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

