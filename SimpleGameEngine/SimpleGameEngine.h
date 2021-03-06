#pragma once
#include <Windows.h>
#include <chrono>

#include "Math.h"
#include "Draw.h"
#include "Camera.h"

using std::chrono::high_resolution_clock;

class SimpleGameEngine {
private:
    HINSTANCE                         hInstance;
    Draw*                             pDraw;
    Camera*                           pCamera;
    bool                              bResetTime = true;
    high_resolution_clock::time_point tPreviousTime;
    high_resolution_clock::time_point tPreviousKeyPressTime = high_resolution_clock::now();
    
    static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);

public:
    SimpleGameEngine(HINSTANCE, int, int);
    Draw*   getDraw();
    Camera* getCamera();
    int run();
    virtual void init();
    virtual void update(long);
    virtual void draw();

    bool isKeyPressed(int);
    long timeSinceLastKeyPress();
    FVector2D getMousePosition();
};