#include "InputManager.h"

// Module purpose: Adapt Win32 virtual key codes (WPARAM) into game-friendly characters.
// This is a translation layer that isolates platform-specific details from the game logic.

InputManager::InputManager() : m_keyHistory()
{
    // Member initializer list preferred over assignment for efficiency and const member initialization
}

// Called by GameWindow's HandleMessage() on WM_KEYDOWN - separates window message routing from input logic
void InputManager::OnKeyDown(WPARAM wParam)
{
    char key = VirtualKeyToChar(wParam);
    // Only store recognized keys; '\0' indicates unmapped key (e.g., arrow keys)
    if (key != '\0') {
        m_keyHistory.add(key);
    }
}

// Placeholder for key-up tracking; Win32 sends WM_KEYUP but most games only care about WM_KEYDOWN
void InputManager::OnKeyUp(WPARAM wParam)
{
    // Future: implement continuous key states (e.g., is_key_held) here if needed
}

// Returns the most recent keystroke without removing it; uses back() for O(1) access instead of front()
std::optional<char> InputManager::GetPressedKey()
{
    if (m_keyHistory.is_empty()) {
        return std::nullopt;  // std::nullopt signals "no value" without sentinel values
    }
    
    const auto& history = m_keyHistory.get_history();
    return history.back();  // LIFO - most recent key is what the player likely intended
}

// Consumes and returns the most recent keystroke; removes the key from history
std::optional<char> InputManager::GetAndConsumeKey()
{
    if (m_keyHistory.is_empty()) {
        return std::nullopt;
    }
    
    const auto& history = m_keyHistory.get_history();
    char key = history.back();
    
    // Remove the key so it's not returned again next frame
    m_keyHistory.clear();  // Or add a pop_back() method to KeyHistory
    
    return key;
}

// Const member function signals to callers that this doesn't modify the InputManager state
const KeyHistory& InputManager::GetKeyHistory() const
{
    return m_keyHistory;
}

// Clears recorded input history; useful for resetting between command sequences
void InputManager::ClearHistory()
{
    m_keyHistory.clear();
}

// Translates platform-specific Win32 virtual key codes to portable ASCII/UTF-8 characters.
// This is where we hide Win32 dependency; game code never sees VK_SPACE, only ' '.
char InputManager::VirtualKeyToChar(WPARAM vKey) const
{
    // Direct range checks preferred over virtual function dispatch for simple key mapping
    if (vKey >= 'A' && vKey <= 'Z') {
        return static_cast<char>(vKey);
    }
    
    if (vKey >= '0' && vKey <= '9') {
        return static_cast<char>(vKey);
    }
    
    // Switch statement for special keys - clearer than nested if/else for enumeration
    switch (vKey) {
        case VK_SPACE:
            return ' ';
        case VK_RETURN:
            return '\n';
        case VK_TAB:
            return '\t';
        case VK_BACK:
            return '\b';
        case VK_ESCAPE:
            return '\x1b';
        default:
            return '\0';  // Sentinel value: '\0' means "this key is not mapped"
    }
}

// Helper method for future filtering; separates alphanumeric detection from conversion logic
bool InputManager::IsAlphanumericKey(WPARAM vKey) const
{
    return (vKey >= 'A' && vKey <= 'Z') || (vKey >= '0' && vKey <= '9');
}

void InputManager::OnMouseMove(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}

void InputManager::OnRightMouseDown()
{
    m_rightMouseDown = true;
}

void InputManager::OnRightMouseUp()
{
    m_rightMouseDown = false;
}

bool InputManager::IsRightMouseDown() const
{
    return m_rightMouseDown;
}

MouseDelta InputManager::GetMouseDelta()
{
    MouseDelta delta;
    // Calculate the difference between current and last frame
    delta.x = static_cast<float>(m_mouseX - m_lastMouseX);
    delta.y = static_cast<float>(m_mouseY - m_lastMouseY);

    // Reset the last position to current so the delta is only read once per movement
    m_lastMouseX = m_mouseX;
    m_lastMouseY = m_mouseY;

    return delta;
}