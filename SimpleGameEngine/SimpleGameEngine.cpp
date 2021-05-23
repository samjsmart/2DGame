#include "SimpleGameEngine.h"

SimpleGameEngine::SimpleGameEngine(HINSTANCE hInstance, int width, int height)
    : hInstance(hInstance) {
    pDraw = new Draw(hInstance, wndProc, this, width, height);
    pCamera = new Camera(FVector2D(0, 0));
}

LRESULT CALLBACK SimpleGameEngine::wndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    LRESULT result                      = 0;
    BOOL    bHandled                    = false;
    SimpleGameEngine* pSimpleGameEngine = NULL;

    // If creation event, store SimpleGameEnine pointer as Window attribute
    if (uMessage == WM_CREATE) {
        pSimpleGameEngine = (SimpleGameEngine*)((LPCREATESTRUCT)lParam)->lpCreateParams;

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pSimpleGameEngine);

        result = 1;
    }
    // Otherwise process messages as normal
    else {
        pSimpleGameEngine = (SimpleGameEngine*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

        if (pSimpleGameEngine) {
            switch (uMessage) {
                case WM_PAINT: {

                    if (pSimpleGameEngine->bResetTime) {
                        pSimpleGameEngine->tPreviousTime = high_resolution_clock::now();
                        pSimpleGameEngine->bResetTime = false;
                    }
                    else {
                        auto tNow = high_resolution_clock::now();
                        auto tDelta = std::chrono::duration_cast<std::chrono::milliseconds>(tNow - pSimpleGameEngine->tPreviousTime).count();
                        pSimpleGameEngine->update((long)tDelta);
                        pSimpleGameEngine->tPreviousTime = tNow;
                    }

                    Draw* pDraw = pSimpleGameEngine->getDraw();
                    if (pDraw) {
                        pDraw->beginPaint();
                        pSimpleGameEngine->draw();
                        pDraw->endPaint();
                    }
                   
                    result = 0;
                    bHandled = true;
                }
                break;

                case WM_DESTROY: {
                    PostQuitMessage(0);
                    result = 0;
                    bHandled = true;
                }
                break;
            }
        }

        if (!bHandled)
            result = DefWindowProc(hWnd, uMessage, wParam, lParam);
    }

    return result;
}

Draw* SimpleGameEngine::getDraw() {
    return pDraw;
}

Camera* SimpleGameEngine::getCamera() {
    return pCamera;
}

int SimpleGameEngine::run() {
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void SimpleGameEngine::update(long lTimeElapsed) {}

void SimpleGameEngine::draw() {}

void SimpleGameEngine::init() {}

bool SimpleGameEngine::isKeyPressed(int key) {
    bool bPressed = GetKeyState(key) & 0x8000;

    if (bPressed) {
        tPreviousKeyPressTime = tPreviousTime;
    }

    return bPressed;
}

long SimpleGameEngine::timeSinceLastKeyPress() {
    auto tNow = high_resolution_clock::now();
    auto tDelta = std::chrono::duration_cast<std::chrono::milliseconds>(tNow - tPreviousKeyPressTime).count();
    return (long)tDelta;
}

FVector2D SimpleGameEngine::getMousePosition() {
    FVector2D position(0, 0);

    POINT p;
    if (!GetCursorPos(&p)) {
        return position;
    }

    if (!ScreenToClient(pDraw->getHwnd(), &p)) {
        return position;
    }

    position.X = p.x;
    position.Y = p.y;

    return position;
}