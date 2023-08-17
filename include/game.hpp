#ifndef GAME_HPP
#define GAME_HPP

#include <string>

class Game
{
public:
    void initializeWindow(const std::string& title) const;
    void run() const;
    void closeWindow() const;
};

#endif