#include "../include/game_camera.hpp"

WorldCamera::WorldCamera()
    : target(nullptr)
{
    camera.zoom = 4;
}

void
WorldCamera::update()
{
    if (target == nullptr)
        return;

    if (IsKeyDown(KEY_ONE))
        camera.zoom++;

    if (IsKeyDown(KEY_TWO) && camera.zoom > 1.0f)
        camera.zoom--;

    camera.target = {target->position.x, target->position.y};
    camera.offset = {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
}