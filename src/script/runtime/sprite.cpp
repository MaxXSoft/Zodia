#include "script/runtime/sprite.h"

#include <cassert>

#include "script/runtime/helper/bounding.h"
#include "script/runtime/helper/collide.h"
#include "sprite/number.h"

using namespace ionia::vm;

void SpriteRuntime::InitOpMap() {
  ops_["x"] = SpriteOp::SetX;
  ops_["x?"] = SpriteOp::GetX;
  ops_["y"] = SpriteOp::SetY;
  ops_["y?"] = SpriteOp::GetY;
  ops_["width"] = SpriteOp::SetWidth;
  ops_["width?"] = SpriteOp::GetWidth;
  ops_["height"] = SpriteOp::SetHeight;
  ops_["height?"] = SpriteOp::GetHeight;
  ops_["visible"] = SpriteOp::SetVisible;
  ops_["visible?"] = SpriteOp::GetVisible;
  ops_["remove"] = SpriteOp::Remove;
  ops_["number"] = SpriteOp::SetNumber;
  ops_["number?"] = SpriteOp::GetNumber;
  children_["bounding"] = std::make_unique<BoundingRuntime>(*this);
  children_["collide?"] = std::make_unique<CollideRuntime>(*this, host_);
  // TODO: foreach, create and sprite.method(id, ...)
  children_["foreach!"] = nullptr;
  children_["create"] = nullptr;
}

RuntimeRef SpriteRuntime::GetChild(const std::string &name) {
  if (!sprite_ || !op_.empty()) return nullptr;
  // try to find in 'children_'
  auto it = children_.find(name);
  if (it != children_.end()) {
    it->second->ResetState();
    return it->second.get();
  }
  else {
    op_ = name;
    return this;
  }
}

RuntimeRef SpriteRuntime::GetElement(const std::string &name) {
  if (sprite_ || !op_.empty()) return nullptr;
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

bool SpriteRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get opcode by name
  auto it = ops_.find(op_);
  if (it == ops_.end()) return false;
  // get paramenter from stack
  int arg;
  switch (it->second) {
    case SpriteOp::SetX: case SpriteOp::SetY: case SpriteOp::SetWidth:
    case SpriteOp::SetHeight: case SpriteOp::SetVisible:
    case SpriteOp::SetNumber: {
      if (vals.top().env) return false;
      arg = vals.top().value;
      vals.pop();
      break;
    }
    default:;
  }
  // perform operations
  ret.value = 0;
  switch (it->second) {
    case SpriteOp::SetX: sprite_->set_x(arg); break;
    case SpriteOp::GetX: ret.value = sprite_->x(); break;
    case SpriteOp::SetY: sprite_->set_y(arg); break;
    case SpriteOp::GetY: ret.value = sprite_->y(); break;
    case SpriteOp::SetWidth: sprite_->set_width(arg); break;
    case SpriteOp::GetWidth: ret.value = sprite_->width(); break;
    case SpriteOp::SetHeight: sprite_->set_height(arg); break;
    case SpriteOp::GetHeight: ret.value = sprite_->height(); break;
    case SpriteOp::SetVisible: sprite_->set_visible(arg); break;
    case SpriteOp::GetVisible: ret.value = sprite_->visible(); break;
    case SpriteOp::Remove: {
      sprite_->parent()->RemoveObject(sprite_.get());
      break;
    }
    case SpriteOp::SetNumber: {
      auto num_ptr = dynamic_cast<Number *>(sprite_.get());
      if (!num_ptr) return false;
      num_ptr->set_number(static_cast<std::uint32_t>(arg));
      break;
    }
    case SpriteOp::GetNumber: {
      auto num_ptr = dynamic_cast<Number *>(sprite_.get());
      if (!num_ptr) return false;
      ret.value = static_cast<int>(num_ptr->number());
      break;
    }
    default: assert(false); return false;
  }
  ret.env = nullptr;
  return true;
}
