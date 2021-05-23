#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm>


#include "SimpleGameEngine.h"
#include "Tile.h"
#include "SpriteMap.h"

class MyGame : public SimpleGameEngine
{
    std::vector<Tile> tiles;
    SpriteMap*        tileMap;
    long              lastInputTime;

    // testing
    int               tileIndex = 20;

    using SimpleGameEngine::SimpleGameEngine;
    virtual void update(long);
    virtual void draw();

public:
    virtual void init();
};

