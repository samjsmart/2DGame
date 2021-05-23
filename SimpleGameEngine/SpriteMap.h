#pragma once
#include <vector>

#include "Math.h"
#include "Draw.h"

class SpriteMap
{
public:
    FVector2D    tileSize;
    ID2D1Bitmap* tileMap;
    std::vector<D2D1_RECT_F> tiles;

    SpriteMap() : tileSize(FVector2D(0, 0)), tileMap(NULL) {};
    SpriteMap(FVector2D tileSize, ID2D1Bitmap* tileMap);
};

