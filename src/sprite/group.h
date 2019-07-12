#ifndef ZODIA_SPRITE_GROUP_H_
#define ZODIA_SPRITE_GROUP_H_

#include <list>
#include <utility>
#include <functional>

#include "sprite/sprite.h"

// represents a group of sprites
class Group : public Sprite, public ParentInterface {
 public:
  using Callback = std::function<void(const SpritePtr &)>;

  Group(ParentPtr parent) : Sprite(parent) { set_visible(true); }

  // create a new sprite in group
  template <typename T, typename... Args>
  void CreateSprite(Args &&... args) {
    sprites_.push_back(
        {std::make_shared<T>(this, std::forward<Args>(args)...), true});
  }

  // add an existing sprite to group
  void AddSprite(const SpritePtr &sprite) {
    sprites_.push_back({sprite, true});
  }

  // clear all sprites in group
  void Clear() { sprites_.clear(); }

  // traversal all sprites in group
  void ForEach(Callback func) {
    for (const auto &i : sprites_) func(i.first);
  }

  void Render(const SDLRendererPtr &renderer) override;
  bool IsCollide(Sprite *sprite) override;
  void RemoveObject(void *id) override;

 private:
  // tagged sprite
  // first: pointer to sprite, second
  // second: whether sprite is alive (false if need to be removed)
  using TaggedSprite = std::pair<SpritePtr, bool>;
  std::list<TaggedSprite> sprites_;
};

#endif  // ZODIA_SPRITE_GROUP_H_
