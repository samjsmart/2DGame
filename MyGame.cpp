#include "MyGame.h"

void MyGame::update(long dTimeElapsed) {
    auto pCamera = getCamera();

    if (isKeyPressed('W')) {
        pCamera->position -= FVector2D(0, 2);
    }

    if (isKeyPressed('A')) {
        pCamera->position -= FVector2D(2, 0);
    }

    if (isKeyPressed('S')) {
        pCamera->position += FVector2D(0, 2);
    }

    if (isKeyPressed('D')) {
        pCamera->position += FVector2D(2, 0);
    }

}

void MyGame::draw() {
    auto pDraw   = this->getDraw();
    auto pCamera = this->getCamera();
    auto brush   = pDraw->createBrush(255, 0, 0);

    FRotator2D r(angle);

    pDraw->drawLine(pCamera->project(lineOrigin + (r * 20.0f)), pCamera->project(lineOrigin + (r * -20.0f)), brush);
}