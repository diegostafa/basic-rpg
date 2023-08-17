#include "../include/main_menu_scene.hpp"
#include "../include/asset_manager.hpp"
#include "../include/game_scene.hpp"
#include "../include/quit_scene.hpp"
#include "../include/settings_scene.hpp"

#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "../deps/ray-widgets/extra/ray_extra_widgets.hpp"
#include "../deps/ray-widgets/utils/ray_widget_utils.hpp"

#include <iostream>

void
MainMenuScene::initialize()
{
    const auto amenu = assets.bindTexture("assets/images/button/ina-box.png");

    const auto mainMenu = new RayCol;
    const auto playBtn  = new RayTextButton("Play");
    const auto quitBtn  = new RayTextButton("Quit");

    ui =
        mainMenu->add({
            playBtn,
            quitBtn,
        });

    RayWidgetState{
        .border = {WHITE, 1, 0}}
        .applyTo({playBtn, quitBtn});

    ui->widgetState.texture = {amenu};

    mainMenu->widgetState.border          = {WHITE, 2};
    mainMenu->widgetState.backgroundColor = {200, 200, 200, 40};

    mainMenu->layoutState.spacingMode = SpacingMode::SPACE_BETWEEN;
    mainMenu->layoutState.spacing     = 15;
    mainMenu->layoutState.padding     = {200, 300, 200, 300};

    mainMenu->flexState.scrollbar.thickness = 0;
    mainMenu->flexState.viewableItems       = 3;

    RayLabelState{
        .textColor = WHITE}
        .applyTo({playBtn->textButtonState.label, quitBtn->textButtonState.label});

    playBtn->buttonEvents.onClick = [this]()
    { this->selectedItem = SelectedMenuItem::NEW_GAME; };

    quitBtn->buttonEvents.onClick = [this]()
    { this->selectedItem = SelectedMenuItem::QUIT_GAME; };
}

void
MainMenuScene::update(SceneManager& sceneManager)
{
    switch (selectedItem)
    {
    case SelectedMenuItem::NEW_GAME:
        selectedItem = SelectedMenuItem::NIL;
        sceneManager.pushScene(new GameScene());
        break;
    case SelectedMenuItem::QUIT_GAME:
        selectedItem = SelectedMenuItem::NIL;
        sceneManager.pushScene(new QuitScene());
        break;
    }
}

void
MainMenuScene::draw()
{
    ClearBackground(LIGHTGRAY);

    auto sw = (float)GetScreenWidth();
    auto sh = (float)GetScreenHeight();

    ui->setSurface({0, 0, sw, sh});
    ui->run();
}

void
MainMenuScene::cleanup()
{
    delete ui;
}
