#ifndef GAME_CAMERA_HPP
#define GAME_CAMERA_HPP

#include "character.hpp"
#include <raylib.h>

class WorldCamera
{
public:
    Camera2D   camera;
    Character* target;

    WorldCamera();

    void update();
};

#endif