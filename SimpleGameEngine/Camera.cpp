#include "Camera.h"

FVector2D Camera::project(FVector2D origin) {
    return origin - position;
}

FVector2D Camera::unproject(FVector2D origin) {
    return origin + position;
}