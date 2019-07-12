#include "scene/scene.h"

void Scene::Render(const SDLRendererPtr &renderer) {
  SDL_RenderClear(renderer.get());
  for (auto &&s : sprites_) {
    for (auto it = s.begin(); it != s.end();) {
      it->second.first->Render(renderer);
      // check if need to be removed
      if (!it->second.second) {
        it = s.erase(it);
      }
      else {
        ++it;
      }
    }
  }
  SDL_RenderPresent(renderer.get());
}

void Scene::RemoveObject(void *id) {
  for (auto &&s : sprites_) {
    for (auto &&i : s) {
      // check if is specific sprite
      if (i.second.first.get() == id) {
        // mark sprite as dead
        i.second.second = false;
        return;
      }
    }
  }
}
