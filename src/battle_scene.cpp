#include "../include/battle_scene.hpp"
#include "../include/asset_manager.hpp"
#include "../include/settings_scene.hpp"

#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "../deps/ray-widgets/extra/ray_extra_widgets.hpp"
#include "../deps/ray-widgets/utils/ray_widget_utils.hpp"

#include <iostream>

BattleScene::BattleScene(Player* player, std::vector<Npc*> enemies)
    : player(player), enemies(enemies)
{
    std::vector<Character*> enemyParty(enemies.begin(), enemies.end());

    battleController = new BattleController();
    battleController->registerParty({player}, false);
    battleController->registerParty(enemyParty, true);
}

void
BattleScene::buildUi()
{
    // structure
    const auto sepCol = new RayCol;

    // enemies half
    const auto enemiesBox           = new RayBox;
    auto       enemiesNamesAndStats = new RayRow;
    auto       enemiesNamesCol      = new RayCol;
    enemiesStats                    = new RayCol;

    // player half
    const auto playerRow = new RayRow;
    const auto statsGrid = new RayGrid(3, 2);
    const auto hpLb      = new RayLabel("HP");
    const auto atkLb     = new RayLabel("ATK");
    const auto defLb     = new RayLabel("DEF");
    actHpLb              = new RayLabel();
    actAtkLb             = new RayLabel();
    actDefLb             = new RayLabel();

    battleLogBox = new RayDialogBox();
    stackBox     = new RayStack();

    // action box
    const auto actionGrid = new RayGrid(2, 2);
    const auto attackBtn  = new RayTextButton("Attack");
    const auto defendBtn  = new RayTextButton("Defend");
    const auto fleeBtn    = new RayTextButton("Flee");

    // target box
    const auto tbox         = new RayCol;
    const auto infoTargetLb = new RayLabel("Select a target");
    targetRow               = new RayRow;

    actionBox =
        actionGrid->add({
            attackBtn,
            defendBtn,
            fleeBtn,
        });

    targetBox =
        tbox->add({
            infoTargetLb,
            targetRow,
        });

    // ui
    ui =
        sepCol->add({
            enemiesBox->add(
                enemiesNamesAndStats->add({
                    enemiesNamesCol,
                    enemiesStats,
                })),
            playerRow->add({
                statsGrid->add({
                    hpLb,
                    actHpLb,
                    atkLb,
                    actAtkLb,
                    defLb,
                    actDefLb,
                }),
                battleLogBox,
                stackBox->add({
                    nullptr,
                    actionBox,
                    targetBox,
                }),
            }),
        });

    std::vector<RayWidget*> enemiesNames = {};
    std::vector<RayWidget*> targetsBtn   = {};

    for (int i = 0; i < enemies.size(); i++)
    {
        auto enemy = enemies[i];

        // health bars
        auto bar                 = new RayProgressBar;
        bar->barState.emptyColor = RED;
        bar->barState.fillColor  = GREEN;
        bar->barState.showValue  = false;
        enemiesHpBars.push_back(bar);

        auto nameLb = new RayLabel(enemy->name + " " + std::to_string(i));
        enemiesNames.push_back(nameLb);

        // targets buttons
        auto btn = new RayTextButton(enemy->name + " " + std::to_string(i));

        btn->textButtonState.label->labelState.textColor = WHITE;

        btn->buttonEvents.onClick = [this, enemy]()
        { battleController->selectedTargets.push_back({enemy, true}); };

        targetsBtn.push_back(btn);
    }

    enemiesNamesCol->add(enemiesNames);
    enemiesStats->add(enemiesHpBars);
    targetRow->add(targetsBtn);

    RayWidgetState{
        .backgroundColor = {0, 0, 0, 140},
        .border          = {BLACK, 1}}
        .applyTo({playerRow, enemiesBox, actionBox, statsGrid, targetBox});

    RayLabelState{
        .textColor = WHITE}
        .propagateFrom(ui);

    sepCol->layoutState.padding     = {10, 10, 10, 10};
    sepCol->layoutState.spacing     = 10;
    sepCol->layoutState.spacingMode = SpacingMode::SPACE_BETWEEN;

    enemiesBox->widgetState.bbox      = {0, 0, 0.5, 40.0f * enemies.size()};
    enemiesBox->widgetState.alignment = {HorizontalAlignment::LEFT, VerticalAlignment::TOP};

    enemiesStats->widgetState.weight      = 4;
    enemiesStats->layoutState.spacing     = 5;
    enemiesStats->layoutState.padding     = {5, 5, 5, 5};
    enemiesStats->layoutState.spacingMode = SpacingMode::SPACE_BETWEEN;

    playerRow->layoutState.padding     = {10, 10, 10, 10};
    playerRow->layoutState.spacing     = 10;
    playerRow->widgetState.bbox        = {0, 0, 0.9, 0.3};
    playerRow->widgetState.alignment.v = VerticalAlignment::BOTTOM;
    statsGrid->widgetState.alignment.h = HorizontalAlignment::LEFT;

    auto& dstate                            = battleLogBox->dialogBoxState;
    dstate.dialogTitle->labelState.fontSize = 24;
    dstate.dialogText->labelState           = dstate.dialogTitle->labelState;

    attackBtn->buttonEvents.onClick = [this]()
    { battleController->selectedAction = BattleAction::ATK; };

    defendBtn->buttonEvents.onClick = [this]()
    { battleController->selectedAction = BattleAction::DEF; };

    fleeBtn->buttonEvents.onClick = [this]()
    { battleController->selectedAction = BattleAction::FLEE; };
}

void
BattleScene::buildFightersUi()
{
    std::vector<RayWidget*> heroWidgetVec  = {};
    std::vector<RayWidget*> enemyWidgetVec = {};

    auto fighters = battleController->fightersQueue;

    const auto mainCol  = new RayCol;
    const auto enemyRow = new RayRow;
    const auto heroRow  = new RayRow;

    for (auto&& fighter : fighters)
    {
        auto box = new RayBox;

        if (!fighter.isEnemy)
        {
            box->widgetState.texture = {playerTex};
            heroWidgetVec.push_back(box);
        }
        else
        {
            box->widgetState.texture = {npcTex};
            enemyWidgetVec.push_back(box);
        }
    }

    fightersUi = mainCol->add({
        enemyRow->add(enemyWidgetVec),
        heroRow->add(heroWidgetVec),
    });

    heroRow->widgetState.bbox  = {0, 0, 0.5, -1};
    enemyRow->widgetState.bbox = {0, 0, 0.5, -1};

    heroRow->widgetState.alignment.h  = HorizontalAlignment::LEFT;
    enemyRow->widgetState.alignment.h = HorizontalAlignment::RIGHT;
}

void
BattleScene::hideActionBox()
{
    stackBox->giveFocus(nullptr);
}

void
BattleScene::showActionSelection()
{
    stackBox->giveFocus(actionBox);
}

void
BattleScene::showTargetSelection()
{
    stackBox->giveFocus(targetBox);
}

void
BattleScene::updateBattleLog(
    const std::vector<std::pair<std::string, std::string>>& script,
    std::function<void()>                                   onDialogEnd)
{
    isBattleLogVisible = true;
    hideActionBox();

    battleLogBox->dialogBoxState.script = script;
    battleLogBox->widgetState.isVisible = false;

    battleLogBox->dialogBoxState.dialogInfo->widgetState.weight = 0;

    battleLogBox->onDialogStart = [this]()
    {
        battleLogBox->fadeIn();
    };

    battleLogBox->onDialogEnd = [this, onDialogEnd]()
    {
        isBattleLogVisible = false;
        battleLogBox->fadeOut();

        if (onDialogEnd)
            onDialogEnd();
    };

    battleLogBox->restart();
}

void
BattleScene::updatePlayerStats()
{
    actHpLb->text  = std::to_string(player->actualStats.hp) + " / " + std::to_string(player->formalStats.hp);
    actAtkLb->text = std::to_string(player->actualStats.atk) + " / " + std::to_string(player->formalStats.atk);
    actDefLb->text = std::to_string(player->actualStats.def) + " / " + std::to_string(player->formalStats.def);
}

void
BattleScene::updateEnemiesStats()
{
    for (int i = 0; i < enemies.size(); i++)
    {
        auto bar = static_cast<RayProgressBar*>(enemiesHpBars[i]);

        bar->barState.minValue = 0;
        bar->barState.maxValue = enemies[i]->formalStats.hp;
        bar->barState.value    = enemies[i]->actualStats.hp;
    }
}

void
BattleScene::initialize()
{
    bgBattle  = assets.bindTexture("assets/images/battle/bgbattle.png");
    playerTex = assets.bindTexture("assets/images/battle/playerbattle.png");
    npcTex    = assets.bindTexture("assets/images/battle/npcbattle.png");

    buildUi();
    buildFightersUi();

    updatePlayerStats();
    updateEnemiesStats();
    updateBattleLog({{"Log", "Battle started!"}});

    battleController->scene = this;
    battleController->startBattle();
}

void
BattleScene::update(SceneManager& sceneManager)
{
    if (isBattleSceneOver)
    {
        sceneManager.popScene();
        return;
    }

    if (isBattleLogVisible)
        return;

    updatePlayerStats();
    updateEnemiesStats();

    auto setSceneOver = [this]()
    { isBattleSceneOver = true; };

    switch (battleController->getBattleResult())
    {
    case BattleResult::ONGOING:
        battleController->updateTurn();
        return;

    case BattleResult::HERO_WON:
        updateBattleLog({{"Log", "You won"}}, setSceneOver);
        break;

    case BattleResult::ENEMY_WON:
        updateBattleLog({{"Log", "The enemy won"}}, setSceneOver);
        break;

    case BattleResult::HERO_FLED:
        updateBattleLog({{"Log", "You ran away"}}, setSceneOver);
        player->body.isUndetectable = true;
        break;

    case BattleResult::ENEMY_FLED:
        updateBattleLog({{"Log", "An enemy left the battle"}}, setSceneOver);
        break;
    }
}

void
BattleScene::draw()
{
    auto sw = (float)GetScreenWidth();
    auto sh = (float)GetScreenHeight();

    ClearBackground(WHITE);

    DrawTexturePro(bgBattle, getTexRec(bgBattle), {0, 0, sw, sh}, {0, 0}, 0, WHITE);

    fightersUi->setSurface({0, 0, sw, sh});
    fightersUi->run();

    ui->setSurface({0, 0, sw, sh});
    ui->run();
}

void
BattleScene::cleanup()
{
    delete ui;
    delete fightersUi;
    delete battleController;
}