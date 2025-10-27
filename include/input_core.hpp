#pragma once
namespace CPPN {
    namespace Input {
        int mouseX = 0;
        int mouseY = 0;
        bool leftMouseDown = false;
        bool rightMouseDown = false;
        bool middleMouseDown = false;
        void MoveMouse(int &newX, int &newY) {
            mouseX=newX;mouseY=newY;
        }
    }
}