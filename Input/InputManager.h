﻿#pragma once

#include "Core/Core.h"

#include <unordered_map>
#include <Windows.h>



class InputManager
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
    
    // return singleton reference
    inline static InputManager& Get()
    {
        static InputManager instance;
        return instance;        
    }


protected:
    void OnKeyDown(int32 KeyCode);
    void OnKeyUp(int32 KeyCode);
    
private:
    std::unordered_map<int, bool> m_keyPressed;
    std::unordered_map<int, bool> m_keyReleased;
    std::unordered_map<int, bool> m_keyHeld;

    std::unordered_map<int, bool> m_mouseButtonPressed;
    std::unordered_map<int, bool> m_mouseButtonReleased;
    std::unordered_map<int, bool> m_mouseButtonHeld;

    int m_mouseX;
    int m_mouseY;

    // singleton
    InputManager();
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    InputManager(InputManager&&) = delete;
    InputManager& operator=(InputManager&&) = delete;

    ~InputManager()
    {
        VG_LOG(LOG_CATEGORY_INPUT, LOG_INFO, "InputManager instance destroyed.");
    }
};
