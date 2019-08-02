#include <iostream>
#include <string>

#include "ionia/src/util/argparse.h"
#include "version.h"
#include "game/gamewin.h"

using namespace std;
using namespace ionia::util;

namespace {

// display version info
void PrintVersion() {
  cout << APP_NAME << " version " << APP_VERSION << endl;
  cout << "A toy 2D game engine, based on SDL2 and Ionia." << endl;
  cout << endl;
  cout << "Copyright (C) 2010-2019 MaxXing, MaxXSoft. License GPLv3.";
  cout << endl;
}

}

int main(int argc, const char *argv[]) {
  // initialize argument parser
  ArgParser argp;
  argp.AddArgument<string>("input", "input Zodia game map file");
  argp.AddOption<bool>("help", "h", "show this message", false);
  argp.AddOption<bool>("version", "v", "show version info", false);
  // parse arguments
  auto ret = argp.Parse(argc, argv);
  if (argp.GetValue<bool>("help")) {
    argp.PrintHelp();
    return 0;
  }
  else if (argp.GetValue<bool>("version")) {
    PrintVersion();
    return 0;
  }
  else if (!ret) {
    cerr << "invalid input, run '";
    cerr << argp.program_name() << " -h' for help" << endl;
    return 1;
  }
  else {
    // run game
    try {
      GameWindow game;
      game.LoadMap(argp.GetValue<string>("input"));
      game.Run();
    }
    catch (const std::runtime_error &e) {
      cerr << "Zodia terminated unexpectedly due to an error: ";
      cerr << e.what() << endl;
      return 1;
    }
  }
  return 0;
}
