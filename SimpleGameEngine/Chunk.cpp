#include "Chunk.h"

Chunk::Chunk(int height, int width, FVector2D position) : height(height), width(width), position(position) {
    tiles.resize(height * width);
}

Tile* Chunk::atLocalPosition(FVector2D pos) {
    return &tiles.at(pos.X * width + pos.Y);
}

Tile* Chunk::atWorldPosition(FVector2D worldPosition) {
    
    
    FVector2D localPosition = FVector2D(
        floor(worldPosition.X - (width * position.X)),
        floor(worldPosition.Y - (height * position.Y))
    );

    return atLocalPosition(localPosition);
}