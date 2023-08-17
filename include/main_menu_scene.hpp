#ifndef MAIN_MENU_SCENE_HPP
#define MAIN_MENU_SCENE_HPP

#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "scene.hpp"

class MainMenuScene : public Scene
{
private:
    RayWidget* ui;

public:
    enum class SelectedMenuItem
    {
        NIL,
        NEW_GAME,
        SETTINGS,
        QUIT_GAME,
    };

    SelectedMenuItem selectedItem = SelectedMenuItem::NIL;

    virtual void initialize() override;
    virtual void update(SceneManager& sceneManager) override;
    virtual void draw() override;
    virtual void cleanup() override;
};

#endif