#ifndef ZODIA_CORE_WINDOW_H_
#define ZODIA_CORE_WINDOW_H_

#include <cstdint>
#include <mutex>
#include <string>

#include "define/sdltype.h"
#include "core/resman.h"
#include "scene/sceneman.h"

class Window {
 public:
  Window()
      : title_("Zodia"), width_(1280), height_(960),
        handle_events_(true), quit_flag_(false), paused_(false),
        res_man_(renderer_), scene_man_(*this),
        window_(nullptr, nullptr), renderer_(nullptr, nullptr) {
    NewInstance();
    Initialize();
  }
  Window(const std::string &title)
      : title_(title), width_(1280), height_(960),
        handle_events_(true), quit_flag_(false), paused_(false),
        res_man_(renderer_), scene_man_(*this),
        window_(nullptr, nullptr), renderer_(nullptr, nullptr) {
    NewInstance();
    Initialize();
  }
  Window(const std::string &title, int width, int height)
      : title_(title), width_(width), height_(height),
        handle_events_(true), quit_flag_(false), paused_(false),
        res_man_(renderer_), scene_man_(*this),
        window_(nullptr, nullptr), renderer_(nullptr, nullptr) {
    NewInstance();
    Initialize();
  }

  virtual ~Window() {
    renderer_.release();
    window_.release();
    ReleaseInstance();
  }

  // render next frame
  void Render();
  // enter main loop of game
  void MainLoop();
  // set quit flag to true, and then quit from game
  void Quit();

  // setters
  void set_title(const std::string &title) {
    title_ = title;
    SDL_SetWindowTitle(window_.get(), title_.c_str());
  }
  void set_paused(bool paused) { paused_ = paused; }
  // getters
  const std::string &title() const { return title_; }
  int width() const { return width_; }
  int height() const { return height_; }
  ResManager &res_man() { return res_man_; }
  SceneManager &scene_man() { return scene_man_; }

 protected:  // event handlers
  // program events
  virtual void QuitEvent() {}
  // window events
  virtual void MinimizedEvent() {}
  virtual void RestoredEvent() {}
  virtual void MouseEnterEvent() {}
  virtual void MouseLeaveEvent() {}
  virtual void GotFocusEvent() {}
  virtual void LostFocusEvent() {}
  virtual void ClosedEvent() {}
  // keyboard events
  virtual void KeyDownEvent(std::uint32_t keycode) {}
  virtual void KeyUpEvent(std::uint32_t keycode) {}
  // mouse events
  virtual void MouseMoveEvent(std::uint32_t state, std::uint32_t x,
                              std::uint32_t y, std::int32_t rx,
                              std::int32_t ry) {}
  virtual void MouseDownEvent(std::uint8_t button, std::uint8_t state,
                              std::uint8_t clicks, std::int32_t x,
                              std::int32_t y) {}
  virtual void MouseUpEvent(std::uint8_t button, std::uint8_t state,
                            std::uint8_t clicks, std::int32_t x,
                            std::int32_t y) {}
  virtual void MouseWhellEvent(std::int32_t x, std::int32_t y,
                               std::uint32_t direction) {}
  // other events
  virtual void CustomEvent(const SDL_Event &event) {}

  // other functions
  virtual void BeforeRender(const SDLRendererPtr &renderer) {}
  virtual void AfterRender(const SDLRendererPtr &renderer) {}

  // set the flag of handle events
  void set_handle_events(bool enabled) { handle_events_ = enabled; }

 private:
  // ref-counted instance control
  static void NewInstance();
  static void ReleaseInstance();

  // SDL window & renderer initializer
  // NOTE: make sure this function will only be called in constructor
  void Initialize();

  // event handling
  void EventDispatch(const SDL_Event &event);

  // shared instance counter
  static int instance_counter_;
  static std::mutex instance_mutex_;
  // properties
  std::string title_;
  const int width_, height_;
  bool handle_events_, quit_flag_, paused_;
  // managers
  ResManager res_man_;
  SceneManager scene_man_;
  // other private variables
  SDLWindowPtr window_;
  SDLRendererPtr renderer_;
};

using WindowPtr = std::shared_ptr<Window>;

#endif  // ZODIA_CORE_WINDOW_H_
