#include "sprite/number.h"

#include <cctype>

void Number::Render(const SDLRendererPtr &renderer) {
  if (!visible()) return;
  uint32_t cur_num = number_;
  SDL_Rect src = {0, clip().y, clip().w, clip().h};
  SDL_Rect dest = {0, y(), clip().w, clip().h};
  for (int i = width() - clip().w; i >= 0; i -= clip().w) {
    src.x = clip().x + (cur_num % 10) * clip().w;
    dest.x = x() + i;
    SDL_RenderCopy(renderer.get(), texture().get(), &src, &dest);
    cur_num /= 10;
  }
}
