#pragma once
#include <map>

#include "Draw.h"
#include "SimpleGameEngine.h"

class Button {
public:
    FVector2D position;
    std::wstring name;
    std::wstring text;
    bool state;

    Button(FVector2D position, std::wstring name, std::wstring text) : position(position), name(name), text(text), state(false) {};
};

class Menu
{
    SimpleGameEngine* pEngine;
    bool              bActive;
    int               iKey;
    int               iFontSize = 20;

    std::map<std::wstring, Button> buttons;

public:
    Menu(SimpleGameEngine* pEngine, int iKey) : pEngine(pEngine), iKey(iKey), bActive(false) {};

    void addButton(std::wstring name, std::wstring text, FVector2D position);
    void draw();
    void tick();
};

