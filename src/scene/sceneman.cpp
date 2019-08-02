#include "scene/sceneman.h"

#include "core/window.h"

bool SceneManager::AddScene(const std::string &id,
                            const GameScenePtr &scene) {
  // insert to hashmap
  auto ret = scenes_.insert({id, scene});
  if (!ret.second) return false;
  // set as current scene
  cur_id_ = id;
  cur_scene_ = scene;
  return true;
}

void SceneManager::QuitGame() {
  game_.Quit();
}

void SceneManager::PauseGame(bool is_paused) {
  game_.set_paused(is_paused);
}

void SceneManager::RenderGame() {
  game_.Render();
}

bool SceneManager::SwitchScene(const std::string &id) {
  // try to find specific scene
  auto it = scenes_.find(id);
  if (it == scenes_.end()) return false;
  // initialize current scene
  cur_id_ = id;
  cur_scene_ = it->second;
  cur_scene_->Reset();
  return true;
}
