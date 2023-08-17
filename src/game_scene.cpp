#include "../include/game_scene.hpp"
#include "../include/asset_manager.hpp"
#include "../include/battle_scene.hpp"
#include "../include/main_menu_scene.hpp"
#include "../deps/ray-widgets/core/ray_core_widgets.hpp"
#include "../deps/ray-widgets/extra/ray_extra_widgets.hpp"
#include <iostream>

void
GameScene::initialize()
{
    std::string initMapName = "castle";
    map                     = new TileMap("assets/tilemaps/" + initMapName + ".tmx", assets);
    player                  = new Player(assets);
    tileEntities            = map->getTileEntities();

    pov.target       = player;
    player->position = map->startingPoints[(int)GetTime() % map->startingPoints.size()];

    // main ui
    const auto mainsector = new RayCol;
    const auto topsector  = new RayBox;
    const auto mapinfobox = new RayBox;
    mapInfoLb             = new RayLabel(initMapName);
    const auto botsector  = new RayRow;
    const auto invbtn     = new RayTextButton("Inventory");

    // menu ui
    const auto menucol   = new RayCol;
    const auto resumeBtn = new RayTextButton("Resume");
    const auto quitBtn   = new RayTextButton("Quit");

    // inventory ui
    const auto invgrid = new RayGrid(3, 3);
    const auto item1   = new RayTextButton("item 1");
    const auto item2   = new RayTextButton("item 2");
    const auto item3   = new RayTextButton("item 3");

    mainUi =
        mainsector->add({
            topsector->add({
                mapinfobox->add(mapInfoLb),
            }),

            botsector->add({
                invbtn,
            }),
        });

    invUi =
        invgrid->add({
            item1,
            item2,
            item3,
        });

    menuUi =
        menucol->add({
            resumeBtn,
            quitBtn,
        });

    // --- style
    RayWidgetState{
        .border = {BLACK, 2}}
        .applyTo({mapinfobox, botsector, invbtn, invgrid, item1, item2, item3, resumeBtn, quitBtn});

    RayLabelState{
        .textColor = WHITE}
        .applyTo({
            mapInfoLb,
            resumeBtn->textButtonState.label,
            quitBtn->textButtonState.label,
        });

    botsector->widgetState.backgroundColor = {20, 20, 20, 180};
    mainsector->layoutState.spacingMode    = SpacingMode::SPACE_EVENLY;
    mainsector->layoutState.spacing        = 20;
    mapinfobox->widgetState.alignment      = {HorizontalAlignment::RIGHT, VerticalAlignment::TOP};
    botsector->widgetState.alignment.v     = VerticalAlignment::BOTTOM;
    botsector->widgetState.alignment.h     = HorizontalAlignment::RIGHT;
    invbtn->widgetState.alignment.h        = HorizontalAlignment::RIGHT;
    invbtn->widgetState.margin             = {10, 10, 10, 10};
    mapinfobox->widgetState.margin.right   = 20;

    botsector->widgetState.bbox  = {0, 0, 200, 100};
    mapinfobox->widgetState.bbox = {0, 0, 0.4, 60};

    invbtn->widgetState.backgroundColor     = {170, 170, 170, 255};
    resumeBtn->buttonState.inactiveColor    = {140, 24, 49, 255};
    quitBtn->buttonState.inactiveColor      = {140, 24, 49, 255};
    mapinfobox->widgetState.backgroundColor = {50, 30, 30, 255};
    invgrid->widgetState.backgroundColor    = {150, 150, 150, 255};

    resumeBtn->widgetState.bbox = {0, 0, 400, 80};
    quitBtn->widgetState.bbox   = {0, 0, 400, 80};

    invgrid->widgetState.margin      = {4, 4, 4, 4};
    invgrid->layoutState.spacing     = 10;
    invgrid->layoutState.spacingMode = SpacingMode::SPACE_EVENLY;

    menucol->widgetState.bbox           = {0, 0, 0.5, 0.5};
    menuUi->widgetState.backgroundColor = {20, 20, 20, 200};

    menuUi->widgetState.isVisible = false;
    invUi->widgetState.isVisible  = false;

    // --- behaviour
    invbtn->buttonEvents.onClick = [this]()
    {
        this->invUi->toggleFade();
    };

    resumeBtn->buttonEvents.onClick = [this]()
    {
        this->menuUi->toggleFade();
    };

    quitBtn->buttonEvents.onClick = [this]()
    {
        this->quit = true;
    };
}

void
GameScene::update(SceneManager& sceneManager)
{
    pov.update();

    if (quit)
    {
        sceneManager.popScene();
        return;
    }

    if (IsKeyPressed(KEY_ESCAPE))
    {
        menuUi->toggleFade();
        return;
    }

    if (menuUi && dialogBoxUi && (menuUi->isVisible() || dialogBoxUi->isVisible()))
        return;

    if (IsKeyPressed(KEY_ENTER))
    {
        Npc* npc      = new Npc(Character::Type::BIRDO, assets);
        npc->position = map->startingPoints[(int)GetTime() % map->startingPoints.size()];
        npc->target   = player;
        npcs.push_back(npc);
    }

    if (IsKeyDown(KEY_P))
        player->actualStats.hp++;

    CollisionDetector cd;
    cd.registerObstacles(map->obstacles);

    for (int i = 0; i < npcs.size(); i++)
    {
        if (npcs[i]->destroy || npcs[i]->actualStats.hp <= 0)
        {
            delete npcs[i];
            npcs.erase(npcs.begin() + i);
        }
    }

    player->update(cd);

    for (auto&& npc : npcs)
        npc->update(cd);

    auto obstacleColliders = cd.detectObstacleCollisions();
    auto charColliders     = cd.detectCharacterCollisions();

    for (auto&& charAndChar : charColliders)
    {
        if ((charAndChar.first.c.type == Character::Type::PLAYER ||
             charAndChar.second.c.type == Character::Type::PLAYER) &&
            player->actualStats.hp > 0)
        {
            sceneManager.pushScene(new BattleScene(player, npcs));
            return;
        }
    }

    for (auto&& collAndDest : map->portals)
    {
        if (CheckCollisionRecs(collAndDest.first, player->getBoundingBox()))
        {
            auto m           = map;
            map              = new TileMap("assets/tilemaps/" + collAndDest.second + ".tmx", assets);
            tileEntities     = map->getTileEntities();
            player->position = map->startingPoints[(int)GetTime() % map->startingPoints.size()];
            mapInfoLb->text  = collAndDest.second;

            delete m;
            break;
        }
    }

    for (auto&& collAndChar : map->interactables)
    {
        if (IsKeyPressed(KEY_Z) && CheckCollisionRecs(collAndChar.first, player->getBoundingBox()))
        {
            delete dialogBoxUi;

            dialogBoxUi = new RayDialogBox();

            auto& dstate                                 = dialogBoxUi->dialogBoxState;
            dstate.dialogUi->widgetState.backgroundColor = {0, 0, 0, 120};
            dstate.dialogUi->widgetState.border          = {{200, 200, 200, 255}, 1, 0};
            dstate.dialogInfo->widgetState.weight        = 2;
            dstate.dialogContent->widgetState.weight     = 5;
            dstate.dialogTitle->widgetState.alignment.h  = HorizontalAlignment::LEFT;
            dstate.dialogText->widgetState.alignment.h   = HorizontalAlignment::LEFT;
            dstate.dialogText->widgetState.alignment.v   = VerticalAlignment::TOP;
            dstate.dialogUi->layoutState.spacing         = 10;
            dstate.dialogUi->layoutState.padding         = {5, 5, 5, 5};
            dstate.dialogContent->layoutState.padding    = {5, 5, 5, 5};
            dstate.dialogInfo->layoutState.padding       = {5, 5, 5, 5};
            dstate.dialogTitle->labelState.textColor     = WHITE;
            dstate.dialogTitle->labelState.fontSize      = 24;
            dstate.dialogText->labelState                = dstate.dialogTitle->labelState;

            dialogBoxUi->widgetState.isVisible = false;
            dialogBoxUi->dialogBoxState.script = {
                {collAndChar.second, "Hello there"},
                {"You", "..."},
            };

            dialogBoxUi->onDialogStart = [this]()
            { dialogBoxUi->fadeIn(); };

            dialogBoxUi->onDialogEnd = [this]()
            { dialogBoxUi->fadeOut(); };

            dialogBoxUi->restart();
        }
    }

    pov.update();
}

void
GameScene::draw()
{
    auto drawables = std::vector<Drawable*>();
    auto sw        = (float)GetScreenWidth();
    auto sh        = (float)GetScreenHeight();

    drawables.reserve(npcs.size() + tileEntities.size() + 1);
    drawables.push_back(player);

    for (auto&& npc : npcs)
        drawables.push_back(npc);

    for (auto&& tile : tileEntities)
        drawables.push_back(&tile);

    const auto yCoordOrderingDesc = [](const Drawable* a, const Drawable* b)
    { return a->position.y < b->position.y; };

    std::sort(drawables.begin(), drawables.end(), yCoordOrderingDesc);

    ClearBackground(BLACK);

    BeginMode2D(pov.camera);
    {
        map->drawLowerLayers();

        for (auto&& drawable : drawables)
            drawable->draw();

        map->drawUpperLayers();
    }
    EndMode2D();

    mainUi->setSurface({0, 0, sw, sh});
    mainUi->run();

    invUi->setSurface({sw / 3, sh / 3, sw / 3, sh / 3});
    invUi->run();

    if (dialogBoxUi)
    {
        dialogBoxUi->setSurface({0, sh - sh / 4, sw, sh / 4});
        dialogBoxUi->run();
    }

    menuUi->setSurface({0, 0, sw, sh});
    menuUi->run();
}

void
GameScene::cleanup()
{
    delete mainUi;
    delete menuUi;
    delete invUi;
    delete dialogBoxUi;

    delete map;
    delete player;

    for (auto&& npc : npcs)
        delete npc;
}
