#ifndef ZODIA_SCENE_SCENEMAN_H_
#define ZODIA_SCENE_SCENEMAN_H_

#include <vector>

#include "core/window.h"
#include "scene/gamescene.h"

// manager for all game scenes
class SceneManager {
 public:
  SceneManager(Window &game) : game_(game), cur_index_(0) {}

  // add a new game scene to manager
  int AddScene(const GameScenePtr &scene) {
    scenes_.push_back(scene);
    return scenes_.size() - 1;
  }

  // will be called by game window
  // notify current game scene that new frame has begun
  void FrameBegin(KeyStatus key) { scenes_[cur_index_]->FrameBegin(key); }

  // quit from game
  void QuitGame() { game_.Quit(); }
  // pause current game
  void PauseGame(bool is_paused) { game_.set_paused(is_paused); }
  // force rendering current scene
  void RenderGame() { game_.Render(); }
  // switch to specific game scene
  void SwitchScene(int id) {
    cur_index_ = id;
    scenes_[cur_index_]->Reset();
    game_.set_scene(scenes_[cur_index_]->GetScene());
  }

  // getters
  const ScenePtr &cur_scene() const {
    return scenes_[cur_index_]->GetScene();
  }

 private:
  Window &game_;
  std::vector<GameScenePtr> scenes_;
  int cur_index_;
};

#endif  // ZODIA_SCENE_SCENEMAN_H_
