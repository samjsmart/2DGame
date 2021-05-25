#include "Menu.h"

void Menu::addButton(std::wstring name, std::wstring text, FVector2D position) {
    buttons.emplace(name, Button(position, name, text));
}

void Menu::draw() {
    if (!bActive) {
        return;
    }

    Draw* pDraw = pEngine->getDraw();

    auto background = pDraw->createBrush(125, 125, 125, 0.75f);
    auto buttonOff  = pDraw->createBrush(50, 50, 50);
    auto white      = pDraw->createBrush(255, 255, 255);
    auto font       = pDraw->createTextFormat(L"Helvetica", fFontSize);

    pDraw->drawFilledRectangle(FVector2D(0, 0), pDraw->getScreenSize(), background);

    for (auto const buttonPair : buttons) {
        auto button = buttonPair.second;

        FVector2D buttonSize = pDraw->getTextSize(button.text, button.position, font) + FVector2D(20, 20);

        pDraw->drawFilledRectangle(button.position + FVector2D(-1, -1), button.position + buttonSize + FVector2D(1, 1), white);
        pDraw->drawFilledRectangle(button.position, button.position + buttonSize, buttonOff);
        pDraw->drawText(button.text, button.position + FVector2D(10, 10), font, white);
    }

}

void Menu::tick() {
    long tKeyTime = pEngine->timeSinceLastKeyPress();

    if (pEngine->isKeyPressed(iKey) && tKeyTime > 200) {
        bActive = !bActive;
    }
}