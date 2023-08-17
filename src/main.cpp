#include "../include/game.hpp"

int
main()
{
    Game game;
    game.initializeWindow("2d game template");
    game.run();
    game.closeWindow();
}