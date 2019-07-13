#include "game/gamewin.h"

#include "game/mapparser.h"

void GameWindow::LoadMap(const std::string &file) {
  MapParser parser(*this, host_, file);
  parser.Parse();
}

void GameWindow::Run() {
  MainLoop();
}
