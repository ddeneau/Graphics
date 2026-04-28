#include "GameWindow.h"
#include "InputManager.h"
#include "Graphics/Core/Device.h" 
#include <iostream>

// Module purpose: Bridge between Win32 window API and C++ game engine.
// Uses RAII to manage window lifecycle; static WindowProc callback routes to instance methods via SetWindowLongPtr.

GameWindow::GameWindow(HINSTANCE hInstance, int width, int height, const std::string& title)
    : m_hwnd(nullptr), m_hInstance(hInstance), m_className(L"GameWindowClass"),
      m_width(width), m_height(height), m_isOpen(false),
      m_inputManager(std::make_unique<InputManager>())
{
    // MultiByteToWideChar needed because title is std::string (char) but Win32 expects LPCWSTR (wchar_t)
    int titleLen = MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, nullptr, 0);
    std::wstring wideTitle(titleLen - 1, 0);  // Pre-allocate sized std::wstring; safer than char arrays
    MultiByteToWideChar(CP_ACP, 0, title.c_str(), -1, &wideTitle[0], titleLen);

    // Register window class - aggregate initialization preferred for POD types like WNDCLASS
    WNDCLASS wc = {};  // {} zero-initializes all members; safer than memset
    wc.lpfnWndProc = WindowProc;          // Static function pointer; see WindowProc for routing via SetWindowLongPtr
    wc.hInstance = hInstance;
    wc.lpszClassName = m_className.c_str();  // c_str() required; std::wstring doesn't implicitly convert to LPCWSTR
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // CreateWindow returns nullptr on failure; stored immediately for cleanup in destructor
    m_hwnd = CreateWindow(
        m_className.c_str(),
        wideTitle.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height,
        nullptr, nullptr, hInstance, this  // 'this' passed as lpParam; retrieved in WM_CREATE
    );

    if (m_hwnd != nullptr) {
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)this);
        ShowWindow(m_hwnd, SW_SHOW);
        m_isOpen = true;

        // Create graphics device AFTER window exists
        m_graphicsDevice = std::make_unique<Device>(m_hwnd, m_width, m_height);
    }
}

// Destructor ensures cleanup even if exception thrown; RAII guarantees DestroyWindow and UnregisterClass called
GameWindow::~GameWindow()
{
    if (m_hwnd != nullptr) {
        DestroyWindow(m_hwnd);  // Destroys window and posts WM_DESTROY message
        UnregisterClass(m_className.c_str(), m_hInstance);  // Frees registered window class; needed for cleanup
    }
    // m_inputManager automatically destroyed here via unique_ptr's destructor
}

// Static callback required by Win32 - function pointer signature must match WNDPROC typedef.
// This is the bridge: converts static context (no 'this') to instance method call.
LRESULT CALLBACK GameWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GameWindow* pThis = nullptr;

    if (uMsg == WM_CREATE) {
        // On WM_CREATE, retrieve 'this' from CreateWindow's lpParam (passed as lpCreateParams in CREATESTRUCT)
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<GameWindow*>(pCreate->lpCreateParams);
        // Store 'this' in window user data for all subsequent messages
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
    }
    else {
        // For all other messages, retrieve the stored 'this' pointer from GWLP_USERDATA
        pThis = reinterpret_cast<GameWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    // null check defensive; pThis could be nullptr if SetWindowLongPtr failed or corrupted
    if (pThis != nullptr) {
        return pThis->HandleMessage(uMsg, wParam, lParam);  // Now we have instance context; call member function
    }

    // Default handler for messages received before user data is set or if retrieval fails
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Instance method: now has 'this' and full access to member variables. Switch on message type for routing.
LRESULT GameWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_DESTROY:
            m_isOpen = false;
            PostQuitMessage(0);  // Posts WM_QUIT to message queue, signals ProcessMessages to return false
            return 0;

        case WM_CLOSE:
            m_isOpen = false;
            DestroyWindow(m_hwnd);  // Triggers WM_DESTROY
            return 0;

        case WM_KEYDOWN:
            // Delegate to InputManager - separation of concerns: window handles events, InputManager translates them
            m_inputManager->OnKeyDown(wParam);
            return 0;

        case WM_KEYUP:
            m_inputManager->OnKeyUp(wParam);
            return 0;
        
        case WM_MOUSEMOVE:            
            if (this && this->m_inputManager) {
                int x = LOWORD(lParam);
                int y = HIWORD(lParam);
                m_inputManager->OnMouseMove(x, y);
			}
            return 0;
        
        case WM_RBUTTONDOWN:
            if (this && this->m_inputManager) {
                this->m_inputManager->OnRightMouseDown();
            }
            return 0;
        
        case WM_RBUTTONUP:
            if (this && this->m_inputManager) {
                this->m_inputManager->OnRightMouseUp();
            }
            return 0;

        default:
            return DefWindowProc(m_hwnd, uMsg, wParam, lParam);  // Let Windows handle unrecognized messages
    }
}

// PeekMessage is non-blocking unlike GetMessage; allows game loop to run at fixed timestep
bool GameWindow::ProcessMessages()
{
    MSG msg = {};  // {} zero-initializes; safer than uninitialized MSG
    
    // Loop drains the entire message queue without blocking; PM_REMOVE dequeues each message
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            m_isOpen = false;
            return false;  // Signal game loop to exit
        }

        TranslateMessage(&msg);  // Converts WM_KEYDOWN/WM_KEYUP to WM_CHAR for text input (not needed here but standard practice)
        DispatchMessage(&msg);   // Routes message back to WindowProc
    }

    return m_isOpen;  // Continue running while window is open
}

// Trivial accessors; compiler inlines these, no function call overhead
HWND GameWindow::GetHwnd() const
{
    return m_hwnd;
}

int GameWindow::GetWidth() const
{
    return m_width;
}

int GameWindow::GetHeight() const
{
    return m_height;
}

bool GameWindow::IsOpen() const
{
    return m_isOpen;
}

// Returns raw pointer to InputManager; ownership remains with this GameWindow via unique_ptr
InputManager* GameWindow::GetInputManager()
{
    return m_inputManager.get();  // get() returns underlying raw pointer from unique_ptr
}

Device* GameWindow::GetGraphicsDevice()
{
    return m_graphicsDevice.get();
}