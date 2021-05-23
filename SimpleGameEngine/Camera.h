#pragma once
#include "Math.h"

class Camera
{
public:
    FVector2D position;

    Camera(FVector2D position) : position(position) {};
    FVector2D project(FVector2D);
    FVector2D unproject(FVector2D);
};

