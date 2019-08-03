#include "script/runtime/scene.h"

#include <cassert>

using namespace ionia::vm;

void SceneRuntime::InitOpMap() {
  ops_["quit"] = SceneOp::Quit;
  ops_["pause"] = SceneOp::Pause;
  ops_["render"] = SceneOp::Render;
  ops_["switch"] = SceneOp::Switch;
}

RuntimeRef SceneRuntime::GetChild(const std::string &name) {
  if (op_.empty()) {
    op_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

RuntimeRef SceneRuntime::GetElement(const std::string &name) {
  if (selected_.empty()) {
    selected_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

bool SceneRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get opcode by name
  auto it = ops_.find(op_);
  if (it == ops_.end()) return false;
  // perform operations
  ret.value = 0;
  switch (it->second) {
    case SceneOp::Quit: host_.scene_man().QuitGame(); break;
    case SceneOp::Pause: {
      // get parameter from stack
      if (vals.top().env) return false;
      bool pause = vals.top().value;
      vals.pop();
      // pause game
      host_.scene_man().PauseGame(pause);
      break;
    }
    case SceneOp::Render: host_.scene_man().RenderGame(); break;
    case SceneOp::Switch: {
      if (selected_.empty()) return false;
      ret.value = host_.scene_man().SwitchScene(selected_);
      break;
    }
    default: assert(false); return false;
  }
  ret.env = nullptr;
  return true;
}
