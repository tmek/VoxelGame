


#include <Input/InputManager.h>



InputManager::InputManager(): m_windowCenterX{0}, m_windowCenterY{0}
{
    TE_LOG(LogInput, Log, TEXT("InputManager instance created."));

    // Initialize mouse position
    m_mouseX = 0;
    m_mouseY = 0;
}

InputManager::~InputManager()
{
    TE_LOG(LogInput, Log, TEXT("InputManager instance destroyed."));
}

void InputManager::Update( UINT uMsg, WPARAM wParam, LPARAM lParam ) {
    // Clear previous input states
    m_mouseButtonPressed.clear();
    m_mouseButtonReleased.clear();

    int KeyCode = static_cast<int>(wParam);
    
    switch (uMsg) {
    case WM_KEYDOWN:
        m_keyPressed.clear();
        m_keyReleased.clear();
        m_keyPressed[KeyCode] = true;
        m_keyHeld[KeyCode] = true;
        OnKeyDown(KeyCode);
        break;
    case WM_KEYUP:
        m_keyPressed.clear();
        m_keyReleased.clear();
        m_keyReleased[KeyCode] = true;
        m_keyHeld[KeyCode] = false;
        OnKeyUp(KeyCode);
        break;
    case WM_LBUTTONDOWN:
        m_mouseButtonPressed[VK_LBUTTON] = true;
        m_mouseButtonHeld[VK_LBUTTON] = true;
        break;
    case WM_LBUTTONUP:
        m_mouseButtonReleased[VK_LBUTTON] = true;
        m_mouseButtonHeld[VK_LBUTTON] = false;
        break;
    case WM_RBUTTONDOWN:
        m_mouseButtonPressed[VK_RBUTTON] = true;
        m_mouseButtonHeld[VK_RBUTTON] = true;
        break;
    case WM_RBUTTONUP:
        m_mouseButtonReleased[VK_RBUTTON] = true;
        m_mouseButtonHeld[VK_RBUTTON] = false;
        break;
    case WM_MOUSEMOVE:
        m_mouseX = LOWORD(lParam);
        m_mouseY = HIWORD(lParam);
        break;
    default:
        break;
    }
}

bool InputManager::IsKeyPressed(int key) {
    return m_keyPressed[key];
}

bool InputManager::IsKeyReleased(int key) {
    return m_keyReleased[key];
}

bool InputManager::IsKeyHeld(int key) {
    return m_keyHeld[key];
}

bool InputManager::IsMouseButtonPressed(int button) {
    return m_mouseButtonPressed[button];
}

bool InputManager::IsMouseButtonReleased(int button) {
    return m_mouseButtonReleased[button];
}

bool InputManager::IsMouseButtonHeld(int button) {
    return m_mouseButtonHeld[button];
}

void InputManager::GetMousePosition(int& x, int& y) const
{
    // x = m_mouseX;
    // y = m_mouseY;
    // get mouse position relative to window center
    x = m_mouseX - m_windowCenterX;
    y = m_mouseY - m_windowCenterY;
}


void InputManager::ClearKeyPressed(int KeyCode)
{
    m_keyPressed[KeyCode] = false;
}

void InputManager::ShowCursor(bool show)
{
    ::ShowCursor(show);
}

void InputManager::CenterMouse(HWND hWnd)
{
    // get full size of  window (including title bar etc.)
    RECT rect;
    GetWindowRect(hWnd, &rect);
    // calculate client area
    GetClientRect(hWnd, &rect);    
    
    
    m_windowCenterX = (rect.left + rect.right) / 2;
    m_windowCenterY = (rect.top + rect.bottom) / 2;
    
    SetCursorPos(m_windowCenterX, m_windowCenterY);
}

InputManager& InputManager::Get()
{
    static InputManager instance;
    return instance;        
}


void InputManager::OnKeyDown(int32 KeyCode)
{
    //TE_LOG(LogTemp, Log, "Key Down: %d", KeyCode); //TODO: add verbose logging?

    // escape key
    if (KeyCode == VK_ESCAPE)
    {
        GIsRequestingExit = true;
    }
}

void InputManager::OnKeyUp(int32 KeyCode)
{
    //TE_LOG(LogTemp, Log, "Key Up: %d", KeyCode); //TODO: add verbose logging?
}
