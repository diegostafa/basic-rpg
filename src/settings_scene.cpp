#include "../include/settings_scene.hpp"
#include "../include/asset_manager.hpp"
#include "../include/main_menu_scene.hpp"

void
SettingsScene::initialize()
{
}

void
SettingsScene::update(SceneManager& sceneManager)
{
    if (IsKeyPressed(KEY_ESCAPE))
    {
        sceneManager.popScene();
        return;
    }
}

void
SettingsScene::draw()
{
    ClearBackground(BLACK);
    DrawRectangle(10, 10, 300, 60, RED);
    DrawText("SETTINGS", 20, 20, 20, BLACK);
}

void
SettingsScene::cleanup()
{
}
