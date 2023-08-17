#include "../include/scene_manager.hpp"
#include <iostream>

SceneManager::~SceneManager()
{
    for (auto&& scene : activeScenes)
    {
        if (scene != nullptr)
        {
            scene->cleanup();
            delete scene;
        }
    }

    activeScenes.clear();
}

void
SceneManager::pushScene(Scene* newScene, bool fade)
{
    if (fade)
        fadeTimer.reset().start();

    newScene->initialize();
    activeScenes.push_back(newScene);
}

void
SceneManager::popScene(bool fade)
{
    if (fade)
        fadeTimer.reset().start();

    auto poppedScene = activeScenes.back();
    poppedScene->cleanup();
    delete poppedScene;
    activeScenes.pop_back();
}

void
SceneManager::stop()
{
    activeScenes.push_back(nullptr);
}

void
SceneManager::update()
{
    const auto prevScene = activeScenes.back();
    activeScenes.back()->update(*this);

    if (hasStopped())
    {
        sceneChanged = true;
        return;
    }

    const auto currScene = activeScenes.back();
    sceneChanged         = prevScene != currScene;
}

void
SceneManager::draw()
{
    activeScenes.back()->draw();
}

bool
SceneManager::hasStopped() const
{
    return activeScenes.empty() || activeScenes.back() == nullptr;
}

bool
SceneManager::hasSceneChanged() const
{
    return sceneChanged;
}
