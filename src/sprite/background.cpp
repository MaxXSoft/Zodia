#include "sprite/background.h"

void Background::Render(const SDLRendererPtr &renderer) {
  if (!visible()) return;
  switch (arrange_) {
    case Arrange::Stretch: {
      SDL_RenderCopy(renderer.get(), texture().get(), src_ptr(), nullptr);
      break;
    }
    case Arrange::Tile:
    case Arrange::Fill:
    default: {
      int w, h;
      SDL_GetRendererOutputSize(renderer.get(), &w, &h);
      if (arrange_ == Arrange::Tile) {
        SDL_Rect dest = {0, 0, width(), height()};
        int max_x = SDL_ceilf(w * 1.f / width());
        int max_y = SDL_ceilf(h * 1.f / height());
        for (int i = 0; i < max_x; ++i) {
          for (int j = 0; j < max_y; ++j) {
            dest.x = i * width();
            dest.y = j * height();
            SDL_RenderCopy(renderer.get(), texture().get(), src_ptr(),
                           &dest);
          }
        }
      }
      else {  // Arrange::Fill
        SDL_Rect dest;
        auto ratio = w * 1.0 / h;
        auto ratio_txt = width() * 1.0 / height();
        if (ratio > ratio_txt) {
          dest = {0, 0, w, static_cast<int>(w / ratio_txt)};
          dest.y = (h - dest.h) / 2;
        }
        else {
          dest = {0, 0, static_cast<int>(h * ratio_txt), h};
          dest.x = (w - dest.w) / 2;
        }
        SDL_RenderCopy(renderer.get(), texture().get(), src_ptr(), &dest);
      }
      break;
    }
  }
}
