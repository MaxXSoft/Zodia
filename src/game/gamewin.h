#ifndef ZODIA_GAME_GAMEWIN_H_
#define ZODIA_GAME_GAMEWIN_H_

#include <string>

#include "core/window.h"
#include "script/host.h"

class GameWindow : public Window {
 public:
  GameWindow() {}

  // load map from file
  void LoadMap(const std::string &file);
  // run game
  void Run();

 private:
  ScriptHost host_;
};

#endif  // ZODIA_GAME_GAMEWIN_H_
