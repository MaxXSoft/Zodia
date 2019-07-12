#ifndef ZODIA_CORE_TIMER_H_
#define ZODIA_CORE_TIMER_H_

#include <cstdint>

#include <SDL2/SDL.h>

class Timer {
 public:
  Timer() { Restart(); }

  void Restart() { ticks_ = SDL_GetTicks(); }

  bool CheckInterval(uint32_t interval) {
    return SDL_GetTicks() - ticks_ >= interval;
  }

 private:
  uint32_t ticks_;
};

#endif  // ZODIA_CORE_TIMER_H_
