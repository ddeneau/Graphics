#pragma once
#include <optional>
#include <windows.h>
#include "KeyHistory.h"

struct MouseDelta { float x; float y; };
// Encapsulates Win32 input handling, translating low-level virtual key codes into 
// application-level characters. This separates platform-specific API details from game logic.
class InputManager {
   
public:
    // std::optional avoids null pointer checks; caller explicitly handles the "no input" case
    InputManager();
    virtual ~InputManager() = default;

    // Win32 message handlers - called by GameWindow's static WindowProc callback
    void OnKeyDown(WPARAM wParam);
    void OnKeyUp(WPARAM wParam);
    
    // std::optional<char> makes the absence of input explicit, safer than returning '\0'
    std::optional<char> GetAndConsumeKey();
    std::optional<char> GetPressedKey();
    
    // Returns const reference to avoid accidental modification and unnecessary copies
    const KeyHistory& GetKeyHistory() const;
    void ClearHistory();

    void OnMouseMove(int x, int y);
    void OnRightMouseDown();
    void OnRightMouseUp();
    MouseDelta GetMouseDelta();
    bool IsRightMouseDown() const;

private:
    // Composition over inheritance: KeyHistory handles storage, InputManager handles Win32 translation
    KeyHistory m_keyHistory;
    
    // Private helper - Win32 WPARAM codes don't map 1:1 to characters; explicit conversion needed
    char VirtualKeyToChar(WPARAM vKey) const;
    
    // Helper for future extensibility (e.g., filtering control keys from alphanumeric)
    bool IsAlphanumericKey(WPARAM vKey) const;

    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_lastMouseX = 0;
    int m_lastMouseY = 0;
    bool m_rightMouseDown = false;
};
