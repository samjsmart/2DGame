#pragma once
#include <vector>
#include "Math.h"
#include "Tile.h"

#include "SimpleGameEngine/Math.h"

class Chunk
{
public:
    std::vector<Tile> tiles{ 1 };
    FVector2D position;
    int height;
    int width;

    Chunk() : height(0), width(0), position(FVector2D(0, 0)) {};
    Chunk(int height, int width, FVector2D position);

    Tile* atLocalPosition(FVector2D);
    Tile* atWorldPosition(FVector2D);
};
