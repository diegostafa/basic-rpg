#include "../include/quit_scene.hpp"
#include "../include/asset_manager.hpp"
#include "../include/main_menu_scene.hpp"

void
QuitScene::initialize()
{
}

void
QuitScene::update(SceneManager& sceneManager)
{
    sceneManager.stop();
}

void
QuitScene::draw()
{
}

void
QuitScene::cleanup()
{
}