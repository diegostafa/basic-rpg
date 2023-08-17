#ifndef GAME_SCENE_HPP
#define GAME_SCENE_HPP

#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "../deps/ray-widgets/extra/ray_extra_widgets.hpp"
#include "collision_detector.hpp"
#include "game_camera.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "scene.hpp"
#include "tile_entity.hpp"
#include "tile_map.hpp"

class GameScene : public Scene
{
private:
    RayWidget*    mainUi      = nullptr;
    RayWidget*    menuUi      = nullptr;
    RayWidget*    invUi       = nullptr;
    RayDialogBox* dialogBoxUi = nullptr;

    RayLabel* mapInfoLb = nullptr;

    bool quit = false;

    WorldCamera             pov;
    TileMap*                map;
    Player*                 player;
    std::vector<Npc*>       npcs;
    std::vector<TileEntity> tileEntities;

public:
    virtual void initialize() override;
    virtual void update(SceneManager& sceneManager) override;
    virtual void draw() override;
    virtual void cleanup() override;
};

#endif