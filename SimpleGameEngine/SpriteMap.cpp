#include "SpriteMap.h"

SpriteMap::SpriteMap(FVector2D tileSize, ID2D1Bitmap* tileMap) : tileSize(tileSize), tileMap(tileMap) {
    auto tileMapSize = tileMap->GetSize();

    int rows = floor(tileMapSize.height / tileSize.Y);
    int cols = floor(tileMapSize.width / tileSize.X);

    for (int y(0); y < rows; y++) {
        for (int x(0); x < cols; x++) {
            auto nx = x * tileSize.X;
            auto ny = y * tileSize.X;

            tiles.push_back(D2D1::RectF(nx, ny, nx + tileSize.X, ny + tileSize.Y));
        }
    }
};