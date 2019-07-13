#include "sprite/sprite.h"

#include "util/logger.h"

void Sprite::InitTextureInfo() {
  // check if is invalid texture
  if (!texture_) LOG_ERROR("texture pointer cannot be null");
  // get width and height of texture
  int w, h;
  SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
  // set width and height of current sprite
  if (is_clipped_) {
    if (clip_.x + clip_.w > w || clip_.y + clip_.h > h) {
      LOG_ERROR("invalid clip arguments");
    }
    width_ = clip_.w;
    height_ = clip_.h;
  }
  else {
    width_ = w;
    height_ = h;
  }
  // set up bouding box
  bounding_ = {0, 0, width_, height_};
}

void Sprite::Render(const SDLRendererPtr &renderer) {
  if (visible_) {
    SDLRect dest = {x_, y_, width_, height_};
    SDL_RenderCopy(renderer.get(), texture_.get(), src_ptr(), &dest);
  }
}

bool Sprite::IsCollide(const Sprite &sprite) {
  if (!visible_) return false;
  auto h_dist =
      SDL_abs(x_ + bounding_.x - (sprite.x_ + sprite.bounding_.x));
  auto v_dist =
      SDL_abs(y_ + bounding_.y - (sprite.y_ + sprite.bounding_.y));
  auto half_w = (bounding_.w + sprite.bounding_.w) / 2;
  auto half_h = (bounding_.h + sprite.bounding_.h) / 2;
  return h_dist < half_w && v_dist < half_h;
}
