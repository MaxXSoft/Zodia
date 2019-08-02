#ifndef ZODIA_SCENE_SCENEMAN_H_
#define ZODIA_SCENE_SCENEMAN_H_

#include <string>
#include <unordered_map>

#include "scene/gamescene.h"

// forward declaration
class Window;

// manager for all game scenes
class SceneManager {
 public:
  SceneManager(Window &game) : game_(game) {}

  // add a new game scene to manager
  bool AddScene(const std::string &id, const GameScenePtr &scene);

  // clear all scenes
  void Clear() {
    scenes_.clear();
    cur_id_.clear();
    cur_scene_ = nullptr;
  }

  // will be called by game window
  // notify current game scene that new frame has begun
  void FrameBegin(KeyStatus status) { cur_scene_->FrameBegin(status); }

  // quit from game
  void QuitGame();
  // pause current game
  void PauseGame(bool is_paused);
  // force rendering current scene
  void RenderGame();
  // switch to specific game scene
  bool SwitchScene(const std::string &id);

  // getters
  const std::string &cur_id() const { return cur_id_; }
  const ScenePtr &cur_scene() const { return cur_scene_->GetScene(); }

 private:
  Window &game_;
  std::unordered_map<std::string, GameScenePtr> scenes_;
  std::string cur_id_;
  GameScenePtr cur_scene_;
};

#endif  // ZODIA_SCENE_SCENEMAN_H_
