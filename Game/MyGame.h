#pragma once
#include <map>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>
#include <mutex>

#include "SimpleGameEngine/SimpleGameEngine.h"
#include "SimpleGameEngine/Menu.h"
#include "Chunk.h"
#include "SpriteMap.h"

class MyGame : public SimpleGameEngine
{
    std::map<std::pair<int, int>, Chunk> chunks;
    std::vector<Chunk*> renderChunks;

    Menu*              menu;
    SpriteMap*         tileMap;
    IDWriteTextFormat* textFormat;

    FVector2D mouseWorldPosition;
    FVector2D mouseTilePosition;
    FVector2D mouseChunkPosition;
    FVector2D centerChunkPosition;
    FVector2D screenChunkSize;
    FVector2D cameraTilePosition;
       
    

    // testing
    int tileIndex = 20;
    int tileSize  = 32;
    int chunkSize = 20;
    int fps;
    std::mutex lock;
    FVector2D selectedChunk;

    using SimpleGameEngine::SimpleGameEngine;
    virtual void update(long) override;
    virtual void draw() override;

    Chunk* getChunkAt(FVector2D);
    void generateChunkContents(Chunk* chunk);

    FVector2D worldToChunk(FVector2D worldPosition);
    FVector2D worldToTile(FVector2D worldPosition);
    FVector2D screenToWorld(FVector2D screenPosition);
    Tile* getTileAtWorldPosition(FVector2D worldPosition);

public:
    virtual void init();
};

