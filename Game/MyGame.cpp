#include "MyGame.h"

void MyGame::init() {
    auto pDraw = this->getDraw();

    auto bitmap = pDraw->loadBitmapFromFile(L"tiles.png");
    tileMap     = new SpriteMap(FVector2D(tileSize, tileSize), bitmap);
    textFormat  = pDraw->createTextFormat(L"Veranda", 20);

    auto screenSize = pDraw->getScreenSize();
    screenChunkSize.X = ceil(screenSize.X / (chunkSize * tileSize)) + 2;
    screenChunkSize.Y = ceil(screenSize.Y / (chunkSize * tileSize)) + 1;

    menu = new Menu(this, VK_ESCAPE);

    menu->addButton(L"bDebug", L"Debug", FVector2D(40, 40));
}

void MyGame::update(long dTimeElapsed) {
    lock.lock();

    auto pCamera = getCamera();
    auto pDraw = this->getDraw();

    fps = floor(1000 / dTimeElapsed);

    long tKeyTime = timeSinceLastKeyPress();
    if (GetForegroundWindow() == pDraw->getHwnd()) {

        auto mouseWorldPosition = screenToWorld(getMousePosition());
        mouseTilePosition = worldToTile(mouseWorldPosition);
        mouseChunkPosition = worldToChunk(mouseWorldPosition);

        // Get chunks closest to camera
        auto cameraPosition = pCamera->position + (pDraw->getScreenSize() / 2);
        centerChunkPosition = worldToChunk(cameraPosition);

        FVector2D topLeftChunkPosition;
        
        topLeftChunkPosition.X = ceil(centerChunkPosition.X - (screenChunkSize.X / 2));
        topLeftChunkPosition.Y = ceil(centerChunkPosition.Y - (screenChunkSize.Y / 2));

        renderChunks.clear();
        for (int y = 0; y < screenChunkSize.Y; y++) {
            for (int x = 0; x < screenChunkSize.X; x++) {
                renderChunks.push_back(getChunkAt(FVector2D(x + topLeftChunkPosition.X, y + topLeftChunkPosition.Y)));
            }
        }

        if (isKeyPressed('W')) {
            pCamera->position -= FVector2D(0, 5);
        }

        if (isKeyPressed('A')) {
            pCamera->position -= FVector2D(5, 0);
        }

        if (isKeyPressed('S')) {
            pCamera->position += FVector2D(0, 5);
        }

        if (isKeyPressed('D')) {
            pCamera->position += FVector2D(5, 0);
        }

        if (isKeyPressed(VK_LBUTTON && tKeyTime > 10)) {
            selectedChunk = mouseChunkPosition;

            auto tile = getTileAtWorldPosition(mouseWorldPosition);

            tile->tileId = 60;
        }
    }

    menu->tick();

    lock.unlock();
}

void MyGame::draw() {
    lock.lock();

    auto pDraw   = this->getDraw();
    auto pCamera = this->getCamera();
    auto brush   = pDraw->createBrush(255, 255, 255);
    auto obrush = pDraw->createBrush(255, 0, 255);
    wchar_t buf[256];

    for (auto chunk : renderChunks) {
        if (chunk->tiles.size() < 1000) {
            for (auto& tile : chunk->tiles) {
                pDraw->drawBitmapPart(tileMap->tileMap, pCamera->project(tile.position), FVector2D(tileSize, tileSize), tileMap->tiles.at(tile.tileId));
            }
        }


        auto chunkBrush =  chunk->position == selectedChunk ? obrush : brush;
        auto strokeWidth = chunk->position == selectedChunk ? 2.0f : 1.0f;
        auto topLeft     = chunk->position * (chunkSize * tileSize);
        auto bottomRight = topLeft + (chunkSize * tileSize);

        pDraw->drawLine(
            pCamera->project(topLeft) + FVector2D(0, 1),
            pCamera->project(FVector2D(bottomRight.X, topLeft.Y)) + FVector2D(0, 1),
            chunkBrush,
            strokeWidth);

        pDraw->drawLine(
            pCamera->project(FVector2D(bottomRight.X, topLeft.Y)) + FVector2D(-1, 0),
            pCamera->project(bottomRight) + FVector2D(-1, 0),
            chunkBrush,
            strokeWidth);

        pDraw->drawLine(
            pCamera->project(bottomRight) + FVector2D(0, -1),
            pCamera->project(FVector2D(topLeft.X, bottomRight.Y)) + FVector2D(0, -1),
            chunkBrush,
            strokeWidth);

        pDraw->drawLine(
            pCamera->project(FVector2D(topLeft.X, bottomRight.Y)) + FVector2D(1, 0),
            pCamera->project(topLeft) + FVector2D(1, 0),
            chunkBrush,
            strokeWidth);
    }

    // Mouse Position
    swprintf(buf, 256, L"FPS: %d", fps);
    pDraw->drawText(buf, FVector2D(10, 10), textFormat, brush);

    // Chunk Count Position
    swprintf(buf, 256, L"Chunks: %d", chunks.size());
    pDraw->drawText(buf, FVector2D(10, 30), textFormat, brush);

    // Tile Position
    swprintf(buf, 256, L"Mouse Tile Position: X %.1f, Y %.1f", mouseTilePosition.X, mouseTilePosition.Y);
    pDraw->drawText(buf, FVector2D(10, 50), textFormat, brush);

    // Tile Position
    swprintf(buf, 256, L"Mouse Chunk Position: X %.1f, Y %.1f", mouseChunkPosition.X, mouseChunkPosition.Y);
    pDraw->drawText(buf, FVector2D(10, 70), textFormat, brush);

    // Chunk Position
    swprintf(buf, 256, L"Center Chunk Position: X %.1f, Y %.1f", centerChunkPosition.X, centerChunkPosition.Y);
    pDraw->drawText(buf, FVector2D(10, 90), textFormat, brush);


    // Mark 0,0
    pDraw->drawLine(
        pCamera->project(FVector2D(-10, 0)),
        pCamera->project(FVector2D(10, 0)),
        brush);

    pDraw->drawLine(
        pCamera->project(FVector2D(0, -10)),
        pCamera->project(FVector2D(0, 10)),
        brush);

    menu->draw();

    lock.unlock();
}

Chunk* MyGame::getChunkAt(FVector2D position) {
    try {
        return &chunks.at(std::pair(position.X, position.Y));
    }
    catch (const std::out_of_range) {
        auto newChunk = Chunk(chunkSize, chunkSize, position);
        generateChunkContents(&newChunk);

        chunks.emplace(std::pair(position.X, position.Y), newChunk);

        return &newChunk;
    }
    return nullptr;
}

void MyGame::generateChunkContents(Chunk* chunk) {
    for (int y = 0; y < chunkSize; y++) {
        for (int x = 0; x < chunkSize; x++) {
            
            auto tile = chunk->atLocalPosition(FVector2D(x, y));

            tile->position = FVector2D(
                (chunk->position.X * chunkSize * tileSize) + (x * tileSize), 
                (chunk->position.Y * chunkSize * tileSize) + (y * tileSize)
            );

            int r = (rand() % 10) + 1;
            
            if (r > 2) {
                tile->tileId = 2;
            }
            if (r > 4) {
                tile->tileId = 8;
            }
            if (r > 6) {
                tile->tileId = 27;
            }
            if (r > 8) {
                tile->tileId = 20;
            }
            if (r == 10) {
                tile->tileId = 25;
            }
        }
    }
}

FVector2D MyGame::worldToChunk(FVector2D worldPosition) {
    return FVector2D{
         floor((worldPosition.X / tileSize) / chunkSize),
         floor((worldPosition.Y / tileSize) / chunkSize)
    };
}

FVector2D MyGame::worldToTile(FVector2D worldPosition) {
    return FVector2D{
        floor(worldPosition.X / tileSize),
        floor(worldPosition.Y / tileSize
        )
    };
}

FVector2D MyGame::screenToWorld(FVector2D screenPosition) {
    return getCamera()->unproject(screenPosition);
}

Tile* MyGame::getTileAtWorldPosition(FVector2D worldPosition) {
    FVector2D chunkPosition = worldToChunk(worldPosition);
    Chunk* chunk            = getChunkAt(chunkPosition);

    return chunk->atWorldPosition(worldToTile(worldPosition));
}