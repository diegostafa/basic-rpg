#ifndef QUIT_SCENE_HPP
#define QUIT_SCENE_HPP

#include "scene.hpp"

class QuitScene : public Scene
{
public:
    virtual void initialize() override;
    virtual void update(SceneManager& sceneManager) override;
    virtual void draw() override;
    virtual void cleanup() override;
};

#endif