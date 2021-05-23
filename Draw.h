#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <vector>

#include <iostream>

#include "Math.h"

class Draw {
private:
    WNDPROC                            wndProc;
    LPVOID                             lpParam;
    HWND                               hWnd;
    HINSTANCE                          hInstance;
    ID2D1Factory*                      pD2DFactory;
    ID2D1HwndRenderTarget*             pRenderTarget;
    int                                iWidth, iHeight;
    std::vector<ID2D1SolidColorBrush*> vBrushes;

    void registerWindow();
    void createD2D();
    void clearBrushes();
    void drawLine(float x1, float y1, float x2, float y2, ID2D1SolidColorBrush* pBrush);
    
public:
    Draw(HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, int iWidth, int iHeight);
    ID2D1SolidColorBrush* createBrush(int r, int g, int b);
    HWND getHwnd();
    void drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush);
    void drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush);
    void drawFilledTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush);
    void beginPaint();
    void endPaint();
};