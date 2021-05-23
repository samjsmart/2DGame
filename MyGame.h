#pragma once
#include "SimpleGameEngine.h"

class MyGame : public SimpleGameEngine
{
    FVector2D lineOrigin{ 100, 200 };
    float angle = 20;

    using SimpleGameEngine::SimpleGameEngine;
    virtual void update(long);
    virtual void draw();
};

