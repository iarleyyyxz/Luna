// Keyboard.hpp
#pragma once
#include <unordered_map>

class Keyboard {
public:
    void UpdateKeyState(int key, int action);
    bool IsKeyPressed(int key) const;
    bool WasKeyJustPressed(int key) const;
    bool WasKeyJustReleased(int key) const;
    void EndFrame();  // Chamar no fim de cada frame

private:
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> previousKeyStates;
};
