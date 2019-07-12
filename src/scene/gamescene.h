#ifndef ZODIA_SCENE_GAMESCENE_H_
#define ZODIA_SCENE_GAMESCENE_H_

#include <memory>

#include "define/key.h"
#include "scene/scene.h"

class GameSceneInterface {
public:
    virtual ~GameSceneInterface() = default;
    // event when game scene was reset
    virtual void Reset() = 0;
    // event when there was a new frame
    virtual void FrameBegin(KeyStatus status) = 0;
    // return pointer of scene in current game scene
    virtual const ScenePtr &GetScene() const = 0;
};

using GameScenePtr = std::shared_ptr<GameSceneInterface>;

#endif // ZODIA_SCENE_GAMESCENE_H_
