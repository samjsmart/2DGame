#include "MyGame.h"

void MyGame::init() {
    auto pDraw = this->getDraw();

    auto bitmap = pDraw->loadBitmapFromFile(L"tiles.png");
    
    tileMap = new SpriteMap(FVector2D(32, 32), bitmap);
}

void MyGame::update(long dTimeElapsed) {
    auto pCamera = getCamera();
    auto pDraw = this->getDraw();

    long tKeyTime = timeSinceLastKeyPress();

    if (GetForegroundWindow() == pDraw->getHwnd() && tKeyTime > 50) {

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

        if (isKeyPressed('K') && tKeyTime > 100) {
            if (tileIndex != 0) {
                tileIndex--;
            }
        }

        if (isKeyPressed('L') && tKeyTime > 100) {
            if (tileIndex != tiles.size()) {
                tileIndex++;
            }
        }

        if (isKeyPressed(VK_LBUTTON)) {
            FVector2D worldPosition = pCamera->unproject(getMousePosition());

            worldPosition.X = floor(worldPosition.X / 32) * 32;
            worldPosition.Y = floor(worldPosition.Y / 32) * 32;

            tiles.push_back(Tile(worldPosition, tileIndex));
        }

        if (isKeyPressed(VK_RBUTTON)) {
            FVector2D worldPosition = pCamera->unproject(getMousePosition());

            worldPosition.X = floor(worldPosition.X / 32) * 32;
            worldPosition.Y = floor(worldPosition.Y / 32) * 32;

            auto it = find_if(tiles.begin(), tiles.end(), [&worldPosition](const Tile& t) { return t.position == worldPosition; });

            if (it != tiles.end()) {
                tiles.erase(it);
            }
        }
    }
}

void MyGame::draw() {
    auto pDraw   = this->getDraw();
    auto pCamera = this->getCamera();
    auto brush   = pDraw->createBrush(255, 0, 0);

    pDraw->drawBitmapPart(tileMap->tileMap, FVector2D(5, 5), FVector2D(32, 32), tileMap->tiles.at(tileIndex));

    for (const auto tile : tiles) {
        pDraw->drawBitmapPart(tileMap->tileMap, pCamera->project(tile.position), FVector2D(32, 32), tileMap->tiles.at(tile.tileId));
    }
}