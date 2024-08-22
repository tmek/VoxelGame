#pragma once

#include <unordered_map>
#include "CoreMinimal.h"
#include "Windows/WindowsHWrapper.h"


class CORE_API InputManager
{
public:
    void Update(UINT uMsg, WPARAM wParam, LPARAM lParam);
    
    bool IsKeyPressed(int32 KeyCode);
    bool IsKeyReleased(int32 KeyCode);
    bool IsKeyHeld(int32 KeyCode);
    
    bool IsMouseButtonPressed(int button);
    bool IsMouseButtonReleased(int button);
    bool IsMouseButtonHeld(int button);
    
    void GetMousePosition(int& x, int& y) const;

    void ClearKeyPressed(int KeyCode);

    void ShowCursor(bool show);
    void CenterMouse(HWND hWnd);

    // return singleton reference
    static InputManager& Get();

protected:
    void OnKeyDown(int32 KeyCode);
    void OnKeyUp(int32 KeyCode);
    
private:
// hide Warning C4251 (needs dll-interface) until we have a solution
#pragma warning(push)
#pragma warning(disable:4251)    
    std::unordered_map<int, bool> m_keyPressed;
    std::unordered_map<int, bool> m_keyReleased;
    std::unordered_map<int, bool> m_keyHeld;
    std::unordered_map<int, bool> m_mouseButtonPressed;
    std::unordered_map<int, bool> m_mouseButtonReleased;
    std::unordered_map<int, bool> m_mouseButtonHeld;
#pragma warning(pop)
    
    int m_mouseX;
    int m_mouseY;
    LONG m_windowCenterX;
    LONG m_windowCenterY;

    // singleton
    InputManager();
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    ~InputManager();
};
