#include "core/window.h"

#include <stdexcept>

#include "util/logger.h"

int Window::instance_counter_ = 0;
std::mutex Window::instance_mutex_;

void Window::NewInstance() {
  std::lock_guard<std::mutex> lock(instance_mutex_);
  // check if need to initialize SDL system
  if (!instance_counter_) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
      Logger::LogError("Window::NewInstance");
      throw std::runtime_error("failed to initialize window");
    }
    Logger::LogDebug("SDL has been initialized");
  }
  ++instance_counter_;
  Logger::LogDebug("window created");
}

void Window::ReleaseInstance() {
  Logger::LogDebug("window destroyed");
  std::lock_guard<std::mutex> lock(instance_mutex_);
  --instance_counter_;
  // check if need to destruct SDL system
  if (!instance_counter_) {
    SDL_Quit();
    Logger::LogDebug("SDL has been shutdown");
  }
}

void Window::Initialize() {
  // initialize window
  // TODO: window size!
  window_ = SDLWindowPtr(
      SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, width_ / 2, height_ / 2,
                       SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI),
      SDL_DestroyWindow);
  if (window_) {
    // initialize renderer
    renderer_ =
        SDLRendererPtr(SDL_CreateRenderer(window_.get(), -1,
                                          SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC),
                       SDL_DestroyRenderer);
    if (renderer_) return;
  }
  // error occurred
  ReleaseInstance();
  Logger::LogError("Window::Initialize");
  throw std::runtime_error("failed to initialize window");
}

void Window::EventDispatch(const SDL_Event &event) {
  switch (event.type) {
    case SDL_QUIT: QuitEvent(); break;
    case SDL_KEYDOWN: KeyDownEvent(event.key.keysym.sym); break;
    case SDL_KEYUP: KeyUpEvent(event.key.keysym.sym); break;
    case SDL_WINDOWEVENT: {
      switch (event.window.type) {
        case SDL_WINDOWEVENT_MINIMIZED: MinimizedEvent(); break;
        case SDL_WINDOWEVENT_RESTORED: RestoredEvent(); break;
        case SDL_WINDOWEVENT_ENTER: MouseEnterEvent(); break;
        case SDL_WINDOWEVENT_LEAVE: MouseLeaveEvent(); break;
        case SDL_WINDOWEVENT_FOCUS_GAINED: GotFocusEvent(); break;
        case SDL_WINDOWEVENT_FOCUS_LOST: LostFocusEvent(); break;
        case SDL_WINDOWEVENT_CLOSE: ClosedEvent(); break;
        default: CustomEvent(event); break;
      }
      break;
    }
    case SDL_MOUSEMOTION: {
      MouseMoveEvent(event.motion.state, event.motion.x, event.motion.y,
                     event.motion.xrel, event.motion.yrel);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: {
      MouseDownEvent(event.button.button, event.button.state,
                     event.button.clicks, event.button.x, event.button.y);
      break;
    }
    case SDL_MOUSEBUTTONUP: {
      MouseUpEvent(event.button.button, event.button.state,
                   event.button.clicks, event.button.x, event.button.y);
      break;
    }
    case SDL_MOUSEWHEEL: {
      MouseWhellEvent(event.wheel.x, event.wheel.y, event.wheel.direction);
      break;
    }
    default: CustomEvent(event); break;
  }
}

void Window::Render() {
  // frame begin
  scene_man_.FrameBegin(SDL_GetKeyboardState(nullptr));
  // render current frame
  scene_man_.cur_scene()->Render(renderer_);
}

void Window::MainLoop() {
  while (!quit_flag_) {
    // handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handle_events_) EventDispatch(event);
      if (event.type == SDL_QUIT) return;
    }
    // render
    BeforeRender(renderer_);
    if (!paused_) Render();
    AfterRender(renderer_);
    // delay
    SDL_Delay(10);
  }
}

void Window::Quit() {
  quit_flag_ = true;
}
