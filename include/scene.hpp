#ifndef SCENE_HPP
#define SCENE_HPP

#include "asset_manager.hpp"
#include "scene_manager.hpp"
#include <raylib.h>

class SceneManager;

class Scene
{
protected:
    AssetManager assets;

public:
    virtual ~Scene() = default;

    virtual void initialize()                       = 0;
    virtual void update(SceneManager& sceneManager) = 0;
    virtual void draw()                             = 0;
    virtual void cleanup()                          = 0;
};

#endif