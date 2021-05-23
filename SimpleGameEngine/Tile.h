#pragma once
#include "Math.h"

class Tile
{
public:

    FVector2D position;
    int tileId;

    Tile() : position(FVector2D(0, 0)), tileId(0) {}
    Tile(FVector2D position, int tileId) : position(position), tileId(tileId) {}
};

