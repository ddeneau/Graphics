#pragma once
#include <windows.h>
#include <string>
#include <memory>
#include "InputManager.h"

class InputManager;
class Device;  // Forward declare

class GameWindow {
public:
    GameWindow(HINSTANCE hInstance, int width, int height, const std::string& title);
    ~GameWindow();

    bool ProcessMessages();
    HWND GetHwnd() const;
    int GetWidth() const;
    int GetHeight() const;
    bool IsOpen() const;
    
    InputManager* GetInputManager();
    Device* GetGraphicsDevice();  // New: access GPU device

    GameWindow(const GameWindow&) = delete;
    GameWindow& operator=(const GameWindow&) = delete;

private:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    HWND m_hwnd;
    HINSTANCE m_hInstance;
    std::wstring m_className;
    int m_width;
    int m_height;
    bool m_isOpen;
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<Device> m_graphicsDevice;  // New: GPU device
};