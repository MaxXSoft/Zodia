#include "sprite/group.h"

void Group::Render(const SDLRendererPtr &renderer) {
  if (visible()) {
    for (auto it = sprites_.begin(); it != sprites_.end();) {
      it->first->Render(renderer);
      // check if need to be removed
      if (!it->second) {
        it = sprites_.erase(it);
      }
      else {
        ++it;
      }
    }
  }
}

bool Group::IsCollide(const Sprite &sprite) {
  if (!visible()) return false;
  // check if there is a sprite collides with specific sprite
  for (const auto &i : sprites_) {
    if (i.first->IsCollide(sprite)) return true;
  }
  return false;
}

void Group::RemoveObject(void *id) {
  for (auto &&i : sprites_) {
    if (i.first.get() == id) {
      // mark as dead
      i.second = false;
      return;
    }
  }
}
