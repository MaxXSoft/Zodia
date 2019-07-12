#ifndef ZODIA_SPRITE_NUMBER_H_
#define ZODIA_SPRITE_NUMBER_H_

#include <limits>
#include <cstdint>

#include "sprite/sprite.h"

class Number : public Sprite {
 public:
  Number(ParentPtr parent, const SDLTexturePtr &texture, int cx, int cy,
         int cw, int ch)
      : Sprite(parent, texture, cx, cy, cw, ch) {
    set_number(0);
  }

  void Render(const SDLRendererPtr &renderer) override;

  void set_number(uint32_t number) {
    number_ = number;
    set_width(clip().w * GetNumberLen());
  }
  uint32_t number() const { return number_; }

 private:
  int GetNumberLen() {
    const uint32_t len_list[] = {
      9, 99, 999, 9999, 99999, 999999, 9999999, 99999999, 999999999,
      std::numeric_limits<uint32_t>::max(),
    };
    for (int i = 0; i < sizeof(len_list) / sizeof(uint32_t); ++i) {
      if (number_ <= len_list[i]) return i + 1;
    }
    return 0;
  }

  uint32_t number_;
};

#endif  // ZODIA_SPRITE_NUMBER_H_
