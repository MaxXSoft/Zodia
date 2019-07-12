#ifndef ZODIA_GRAPHIC_SPRITE_H_
#define ZODIA_GRAPHIC_SPRITE_H_

#include <memory>

#include "define/sdltype.h"
#include "define/parent.h"

class Sprite;
using SpritePtr = std::shared_ptr<Sprite>;

class Sprite {
 public:
  Sprite(ParentPtr parent)
      : parent_(parent), is_clipped_(false), visible_(false),
        texture_(nullptr), x_(0), y_(0) {}
  Sprite(ParentPtr parent, const SDLTexturePtr &texture)
      : parent_(parent), is_clipped_(false), visible_(true),
        texture_(texture), x_(0), y_(0) {
    InitTextureInfo();
  }
  Sprite(ParentPtr parent, const SDLTexturePtr &texture, int cx, int cy,
         int cw, int ch)
      : parent_(parent), is_clipped_(true), visible_(true),
        clip_({cx, cy, cw, ch}), texture_(texture), x_(0), y_(0) {
    InitTextureInfo();
  }
  virtual ~Sprite() = default;

  virtual void Render(const SDLRendererPtr &renderer);
  virtual bool IsCollide(Sprite *sprite);

  void set_x(int x) { x_ = x; }
  void set_y(int y) { y_ = y; }
  void set_width(int width) { width_ = width; }
  void set_height(int height) { height_ = height; }
  void set_visible(bool visible) { visible_ = visible; }
  void set_bounding(const SDLRect &bounding) { bounding_ = bounding; }

  int x() const { return x_; }
  int y() const { return y_; }
  int width() const { return width_; }
  int height() const { return height_; }
  bool visible() const { return visible_; }
  const SDLRect &bounding() const { return bounding_; }

 protected:
  ParentPtr parent() const { return parent_; }
  bool is_clipped() const { return is_clipped_; }
  const SDLRect &clip() const { return clip_; }
  const SDLTexturePtr &texture() const { return texture_; }
  const SDLRect *src_ptr() const { return is_clipped_ ? &clip_ : nullptr; }

 private:
  void InitTextureInfo();

  ParentPtr parent_;
  bool is_clipped_, visible_;
  SDLRect clip_, bounding_;
  SDLTexturePtr texture_;
  int x_, y_, width_, height_;
};

#endif  // ZODIA_GRAPHIC_SPRITE_H_
