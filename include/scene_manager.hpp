#ifndef SCENE_MANAGER_HPP
#define SCENE_MANAGER_HPP

#include "scene.hpp"
#include <vector>

#include "../deps/ray-widgets/helpers/include/timer.hpp"

class Scene;

class SceneManager
{
private:
    std::vector<Scene*> activeScenes = {};
    bool                sceneChanged = false;

public:
    Timer fadeTimer = Timer(400);

    ~SceneManager();

    void pushScene(Scene* newScene, bool fade = true);
    void popScene(bool fade = true);
    void stop();
    void draw();
    void update();
    bool hasStopped() const;
    bool hasSceneChanged() const;
};

#endif
