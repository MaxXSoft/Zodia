#ifndef ZODIA_SPRITE_BACKGROUND_H_
#define ZODIA_SPRITE_BACKGROUND_H_

#include "sprite/sprite.h"

// can be used as the background of a scene
class Background : public Sprite {
 public:
  // arrangement of background image
  enum class Arrange : char { Fill, Tile, Stretch };

  Background(const SDLTexturePtr &texture,
             Arrange arrange)
      : Sprite(texture), arrange_(arrange) {}
  Background(const SDLTexturePtr &texture,
             int cx, int cy, int cw, int ch, Arrange arrange)
      : Sprite(texture, cx, cy, cw, ch), arrange_(arrange) {}

  void Render(const SDLRendererPtr &renderer) override;

 private:
  Arrange arrange_;
};

#endif  // ZODIA_SPRITE_BACKGROUND_H_
