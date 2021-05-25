#pragma once
#include "Math.h"

class SimpleGameEngine;
class Camera
{
    SimpleGameEngine* pEngine;

public:
    FVector2D position;

    Camera(SimpleGameEngine* pEngine, FVector2D position) : position(position), pEngine(pEngine) {};
    FVector2D project(FVector2D);
    FVector2D unproject(FVector2D);
};

