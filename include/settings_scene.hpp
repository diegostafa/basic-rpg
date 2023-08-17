#ifndef SETTINGS_SCENE_HPP
#define SETTINGS_SCENE_HPP

#include "scene.hpp"

class SettingsScene : public Scene
{
public:
    virtual void initialize() override;
    virtual void update(SceneManager& sceneManager) override;
    virtual void draw() override;
    virtual void cleanup() override;
};

#endif