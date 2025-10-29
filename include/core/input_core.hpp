#pragma once
#include "unordered_set"
namespace CPPN {
    namespace Input {
        int mouseX = 0;
        int mouseY = 0;
        bool mouse1down = false;
        int keysDown = 0;
        std::unordered_set<std::string> keysPressed;
        void MoveMouse(int &newX, int &newY) {
            mouseX=newX;mouseY=newY;
        }
        void KeyDown(std::string &key) {
            keysPressed.insert(key);
        }
        void KeyUp(std::string &key) {
            if (keysPressed.find(key) != keysPressed.end()) {
                keysPressed.erase(key);
            }
        } 
    }
}