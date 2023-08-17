#include "../include/battle_controller.hpp"
#include <iostream>

void
BattleController::registerParty(std::vector<Character*> party, bool isEnemy)
{
    for (auto&& fighter : party)
        fightersQueue.push_back({fighter, isEnemy});
}

Fighter
BattleController::getFighterToPlay()
{
    return fightersQueue[fighterToPlay];
}

void
BattleController::startBattle()
{
    prepareRound();
    prepareTurn();
}

void
BattleController::prepareRound()
{
    fighterToPlay = 0;

    const auto sortByCharSpeedDesc = [](const Fighter& a, const Fighter& b)
    { return a.character->actualStats.spd < b.character->actualStats.spd; };

    std::sort(fightersQueue.begin(), fightersQueue.end(), sortByCharSpeedDesc);
}

void
BattleController::prepareTurn()
{
    turnPhase      = TurnPhase::START;
    selectedAction = BattleAction::NIL;
    selectedTargets.clear();
}

void
BattleController::updateTurn()
{
    switch (turnPhase)
    {
    case TurnPhase::START:
    {
        scene->updateBattleLog({{"Log", "turn started"}});

        auto fighter = getFighterToPlay();

        if (fighter.character->type == Character::Type::PLAYER)
            turnPhase = TurnPhase::PLAYER;
        else
            turnPhase = TurnPhase::NPC;
        break;
    }

    case TurnPhase::NPC:
    {
        Fighter f;
        for (auto&& fighter : fightersQueue)
        {
            if (!fighter.isEnemy)
                f = fighter;
            break;
        }

        scene->updateBattleLog({{"Log", "NPC is attacking"}});

        submitAttack({
            .damage  = rand() % 10,
            .mpCost  = rand() % 4,
            .targets = {f},
        });

        turnPhase = TurnPhase::END;

        break;
    }
    case TurnPhase::PLAYER:
    {
        turnPhase = TurnPhase::PLAYER_ACTION_SELECTION;
        scene->showActionSelection();
        break;
    }

    case TurnPhase::PLAYER_ACTION_SELECTION:
    {
        switch (selectedAction)
        {
        case BattleAction::NIL:
            break;

        case BattleAction::ATK:
        {
            turnPhase = TurnPhase::PLAYER_TARGET_SELECTION;
            scene->showTargetSelection();
            break;
        }

        case BattleAction::DEF:
            break;

        case BattleAction::FLEE:
            submitFlee();
            turnPhase = TurnPhase::END;
            break;
        }
        break;
    }

    case TurnPhase::PLAYER_TARGET_SELECTION:
    {
        if (!selectedTargets.empty())
        {
            scene->updateBattleLog({{"Log", "Player is attacking"}});

            submitAttack({
                .damage  = 35,
                .mpCost  = 1,
                .targets = selectedTargets,
            });

            turnPhase = TurnPhase::END;
        }
        break;
    }

    case TurnPhase::END:
    {
        scene->updateBattleLog({{"Log", "end turn"}});

        if (++fighterToPlay < fightersQueue.size())
            prepareTurn();
        else if (getBattleResult() == BattleResult::ONGOING)
        {
            prepareRound();
            prepareTurn();
        }
        break;
    }
    }
}

BattleResult
BattleController::getBattleResult()
{
    if (heroFled)
        return BattleResult::HERO_FLED;

    bool allHeroesDead  = true;
    bool allEnemiesDead = true;

    for (auto&& f : fightersQueue)
    {
        if (f.isEnemy)
            allEnemiesDead = allEnemiesDead && f.character->actualStats.hp <= 0;
        else
            allHeroesDead = allHeroesDead && f.character->actualStats.hp <= 0;
    }

    if (allHeroesDead)
        return BattleResult::ENEMY_WON;

    if (allEnemiesDead)
        return BattleResult::HERO_WON;

    return BattleResult::ONGOING;
}

void
BattleController::submitAttack(AttackData atkData)
{
    auto attacker = getFighterToPlay().character;

    for (auto&& t : atkData.targets)
        t.character->actualStats.hp -= atkData.damage;
}

void
BattleController::submitFlee()
{
    heroFled = true;
}