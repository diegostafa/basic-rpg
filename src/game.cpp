#include "../include/game.hpp"
#include "../include/battle_scene.hpp"
#include "../include/game_scene.hpp"
#include "../include/main_menu_scene.hpp"
#include <iostream>
#include <raylib.h>
#include <string>

void
Game::initializeWindow(const std::string& title) const
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1280, 700, title.c_str());
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(KEY_NULL);
    SetWindowMinSize(100, 100);
    SetTargetFPS(60);
    SetTraceLogLevel(LOG_NONE);
}

void
Game::run() const
{
    SceneManager sceneManager;
    sceneManager.pushScene(new MainMenuScene(), false);

    while (!WindowShouldClose() && !sceneManager.hasStopped())
    {
        sceneManager.update();

        if (sceneManager.hasSceneChanged())
            continue;

        BeginDrawing();

        if (!sceneManager.fadeTimer.hasStarted)
            sceneManager.draw();
        else
        {
            auto alpha = (unsigned char)(255 * sceneManager.fadeTimer.progress);

            DrawRectangleRec(
                {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                {0, 0, 0, alpha});

            if (sceneManager.fadeTimer.update().hasElapsed())
                sceneManager.fadeTimer.reset();
        }

        EndDrawing();
    }
}

void
Game::closeWindow() const
{
    CloseWindow();
}