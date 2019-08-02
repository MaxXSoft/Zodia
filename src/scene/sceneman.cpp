#include "scene/sceneman.h"

#include "core/window.h"

void SceneManager::QuitGame() {
  game_.Quit();
}

void SceneManager::PauseGame(bool is_paused) {
  game_.set_paused(is_paused);
}

void SceneManager::RenderGame() {
  game_.Render();
}
