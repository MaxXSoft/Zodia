#include "script/runtime/helper/collide.h"

using namespace ionia::vm;

RuntimeRef CollideRuntime::GetChild(const std::string &name) {
  return nullptr;
}

RuntimeRef CollideRuntime::GetElement(const std::string &name) {
  if (sprite_) return nullptr;
  // get sprite info
  auto &cur_scene = host_.scene_man().cur_id();
  auto &info = host_.sprite_map().at(cur_scene);
  // check if is invalid
  auto it = info.find(name);
  if (it == info.end()) return nullptr;
  // select current sprite and return
  sprite_ = host_.scene_man().cur_scene()->sprite(it->second, name);
  return this;
}

bool CollideRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  ret.env = nullptr;
  ret.value = sprite_rt_.sprite()->IsCollide(*sprite_);
  return true;
}
