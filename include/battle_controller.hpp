#ifndef BATTLE_CONTROLLER_HPP
#define BATTLE_CONTROLLER_HPP

#include "battle_scene.hpp"
#include "character.hpp"
#include <queue>
#include <vector>

class BattleScene;

enum class BattleResult
{
    ONGOING,
    HERO_FLED,
    ENEMY_FLED,
    HERO_WON,
    ENEMY_WON,
};

enum class TurnPhase
{
    NIL,
    START,
    NPC,
    PLAYER,
    PLAYER_ACTION_SELECTION,
    PLAYER_TARGET_SELECTION,
    END,
};

enum class BattleAction
{
    NIL,
    ATK,
    DEF,
    FLEE,
};

struct Fighter
{
    Character* character = nullptr;
    bool       isEnemy   = false;
};

struct AttackData
{
    int damage = 0;
    int mpCost = 0;

    std::vector<Fighter> targets = {};
};

class BattleController
{
private:
public:
    BattleScene*         scene         = nullptr;
    std::vector<Fighter> fightersQueue = {};
    int                  fighterToPlay = 0;
    TurnPhase            turnPhase     = TurnPhase::NIL;

    BattleAction         selectedAction  = BattleAction::NIL;
    std::vector<Fighter> selectedTargets = {};
    bool                 heroFled        = false;

    void         registerParty(std::vector<Character*> party, bool isEnemy);
    void         startBattle();
    void         prepareRound();
    void         prepareTurn();
    void         updateTurn();
    BattleResult getBattleResult();

    Fighter getFighterToPlay();
    void    submitAttack(AttackData atkData);
    void    submitFlee();
};

#endif