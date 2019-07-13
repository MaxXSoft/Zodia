#ifndef ZODIA_SCENE_SCENE_H_
#define ZODIA_SCENE_SCENE_H_

#include <memory>
#include <utility>
#include <string>
#include <map>
#include <vector>
#include <queue>

#include "define/sdltype.h"
#include "define/parent.h"
#include "sprite/sprite.h"

class Scene : public ParentInterface {
 public:
  Scene(int layer_count) { sprites_.resize(layer_count); }

  // create new sprite in scene
  template <typename T, typename... Args>
  void CreateSprite(int layer, const std::string &name, Args &&... args) {
    sprites_[layer][name] = {
        std::make_shared<T>(this, std::forward<Args>(args)...), true};
  }

  // add an existing sprite to scene
  void AddSprite(int layer, const std::string &name,
                 const SpritePtr &sprite) {
    sprite->set_parent(this);
    sprites_[layer][name] = {sprite, true};
  }

  // remove specific sprite in scene
  void RemoveSprite(int layer, const std::string &name) {
    auto &cur_layer = sprites_[layer];
    auto it = cur_layer.find(name);
    if (it != cur_layer.end()) {
      cur_layer.erase(it);
    }
  }

  void RemoveObject(void *id) override;

  // force rendering current scene
  void Render(const SDLRendererPtr &renderer);

  // getters
  const SpritePtr &sprite(int layer, const std::string &name) {
    return sprites_[layer][name].first;
  }

 private:
  // tagged sprite
  // first: pointer to sprite, second
  // second: whether sprite is alive (false if need to be removed)
  using TaggedSprite = std::pair<SpritePtr, bool>;
  using SpriteMap = std::map<std::string, TaggedSprite>;

  std::vector<SpriteMap> sprites_;
};

using ScenePtr = std::shared_ptr<Scene>;

#endif  // ZODIA_SCENE_SCENE_H_
