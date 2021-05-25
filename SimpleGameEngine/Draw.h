#pragma once
#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <atlbase.h>
#include <wincodec.h>
#include <memory>
#include <vector>
#include <string>
#include <iostream>

#include "Math.h"

class SimpleGameEngine;

template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
};

class Draw {
    SimpleGameEngine*                  pEngine;
    WNDPROC                            wndProc;
    LPVOID                             lpParam;
    HWND                               hWnd;
    HINSTANCE                          hInstance;
    ID2D1Factory*                      pD2DFactory;
    ID2D1HwndRenderTarget*             pRenderTarget;
    IDWriteFactory*                    pWriteFactory;
    CComPtr<IWICImagingFactory>        pWICImagingFactory;
    FVector2D                          resolution;
    std::vector<ID2D1SolidColorBrush*> vBrushes;

    void registerWindow();
    void createD2D();
    void clearBrushes();
    void createWICImagingFactory();
    void createIDWriteFactory();
    void drawLine(float x1, float y1, float x2, float y2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
    
public:
    Draw(SimpleGameEngine* pEngine, HINSTANCE hInstance, WNDPROC wndProc, LPVOID lpParam, FVector2D resolution);
    ID2D1SolidColorBrush* createBrush(int r, int g, int b, float a = 1.0f);
    IDWriteTextFormat* createTextFormat(std::wstring fontName, int fontSize);
    ID2D1Bitmap* loadBitmapFromFile(std::wstring path);
    HWND getHwnd();
    void drawLine(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush, float strokeWidth = 1.0f);
    void drawTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush);
    void drawFilledTriangle(FVector2D pt1, FVector2D pt2, FVector2D pt3, ID2D1SolidColorBrush* pBrush);
    void drawFilledRectangle(FVector2D pt1, FVector2D pt2, ID2D1SolidColorBrush* pBrush);
    void drawBitmap(ID2D1Bitmap* pBitmap, FVector2D pt1);
    void drawBitmapPart(ID2D1Bitmap* pBitmap, FVector2D pt1, FVector2D size, D2D1_RECT_F rect);
    void drawText(std::wstring text, FVector2D pt1, IDWriteTextFormat* textFormat, ID2D1SolidColorBrush* pBrush);
    void beginPaint();
    void endPaint();
    FVector2D getScreenSize();
    FVector2D getTextSize(std::wstring text, FVector2D pt1, IDWriteTextFormat* textFormat);
};