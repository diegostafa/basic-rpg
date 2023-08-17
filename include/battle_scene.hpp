#ifndef BATTLE_SCENE_HPP
#define BATTLE_SCENE_HPP

#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "../deps/ray-widgets/extra/ray_extra_widgets.hpp"

#include "battle_controller.hpp"
#include "npc.hpp"
#include "player.hpp"
#include "scene.hpp"
#include <unordered_map>
#include <vector>

class BattleController;

class BattleScene : public Scene
{
private:
    bool isBattleLogVisible = false;
    bool isBattleSceneOver  = false;

    // battle
    BattleController* battleController = nullptr;
    Player*           player           = nullptr;
    std::vector<Npc*> enemies          = {};

    // ui
    RayWidget*    fightersUi   = nullptr;
    RayWidget*    ui           = nullptr;
    RayStack*     stackBox     = nullptr;
    RayWidget*    actionBox    = nullptr;
    RayWidget*    targetBox    = nullptr;
    RayRow*       targetRow    = nullptr;
    RayCol*       enemiesStats = nullptr;
    RayDialogBox* battleLogBox = nullptr;

    // hp bars
    RayLabel*               actHpLb       = nullptr;
    RayLabel*               actAtkLb      = nullptr;
    RayLabel*               actDefLb      = nullptr;
    std::vector<RayWidget*> enemiesHpBars = {};

    // resources
    Texture2D bgBattle;
    Texture2D playerTex;
    Texture2D npcTex;

    void buildUi();
    void buildFightersUi();

public:
    BattleScene(Player* player, std::vector<Npc*> enemies);

    void updateBattleLog(const std::vector<std::pair<std::string, std::string>>& script, std::function<void()> onDialogEnd = nullptr);
    void showActionSelection();
    void showTargetSelection();
    void hideActionBox();
    void updatePlayerStats();
    void updateEnemiesStats();

    virtual void initialize() override;
    virtual void update(SceneManager& sceneManager) override;
    virtual void draw() override;
    virtual void cleanup() override;
};

#endif