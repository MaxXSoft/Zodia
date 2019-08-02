#include "script/runtime/helper/bounding.h"

#include <cassert>

using namespace ionia::vm;

namespace {

inline bool GetArg(VM::ValueStack &vals, int &arg) {
  if (vals.top().env) return false;
  arg = vals.top().value;
  vals.pop();
  return true;
}

}  // namespace

void BoundingRuntime::InitOpMap() {
  ops_[""] = BoundingOp::Call;
  ops_["x"] = BoundingOp::SetX;
  ops_["x?"] = BoundingOp::GetX;
  ops_["y"] = BoundingOp::SetY;
  ops_["y?"] = BoundingOp::GetY;
  ops_["width"] = BoundingOp::SetWidth;
  ops_["width?"] = BoundingOp::GetWidth;
  ops_["height"] = BoundingOp::SetHeight;
  ops_["height?"] = BoundingOp::GetHeight;
}

RuntimeRef BoundingRuntime::GetChild(const std::string &name) {
  if (op_.empty()) {
    op_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

RuntimeRef BoundingRuntime::GetElement(const std::string &name) {
  return nullptr;
}

bool BoundingRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get opcode by name
  auto it = ops_.find(op_);
  if (it == ops_.end()) return false;
  // get paramenter from stack
  int arg;
  switch (it->second) {
    case BoundingOp::SetX: case BoundingOp::SetY:
    case BoundingOp::SetWidth: case BoundingOp::SetHeight: {
      if (!GetArg(vals, arg)) return false;
      break;
    }
  }
  // get bounding from sprite
  auto bounding = sprite_rt_.sprite()->bounding();
  // perform operations
  ret.value = 0;
  switch (it->second) {
    case BoundingOp::Call: {
      if (!GetArg(vals, bounding.x)) return false;
      if (!GetArg(vals, bounding.y)) return false;
      if (!GetArg(vals, bounding.w)) return false;
      if (!GetArg(vals, bounding.h)) return false;
      break;
    }
    case BoundingOp::SetX: bounding.x = arg; break;
    case BoundingOp::GetX: ret.value = bounding.x; break;
    case BoundingOp::SetY: bounding.y = arg; break;
    case BoundingOp::GetY: ret.value = bounding.y; break;
    case BoundingOp::SetWidth: bounding.w = arg; break;
    case BoundingOp::GetWidth: ret.value = bounding.w; break;
    case BoundingOp::SetHeight: bounding.h = arg; break;
    case BoundingOp::GetHeight: ret.value = bounding.h; break;
    default: assert(false); return false;
  }
  sprite_rt_.sprite()->set_bounding(bounding);
  ret.env = nullptr;
  return true;
}
