#include "script/runtime/rand.h"

#include <cstdlib>
#include <ctime>
#include <cassert>

using namespace ionia::vm;

RandRuntime::RandRuntime() {
  Randomize();
  // initialize hashmap
  ops_["randomize"] = RandOp::Randomize;
  ops_["seed"] = RandOp::SetSeed;
  ops_["seed?"] = RandOp::GetSeed;
  ops_["next"] = RandOp::Next;
  ops_["range"] = RandOp::Range;
}

int RandRuntime::Randomize() {
  seed_ = static_cast<unsigned int>(std::time(nullptr));
  std::srand(seed_);
  return seed_;
}

RuntimeRef RandRuntime::GetChild(const std::string &name) {
  if (op_.empty()) {
    op_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

RuntimeRef RandRuntime::GetElement(const std::string &name) {
  return nullptr;
}

bool RandRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get opcode by name
  auto it = ops_.find(op_);
  if (it == ops_.end()) return false;
  ops_.clear();
  // perform operations
  switch (it->second) {
    case RandOp::Randomize: {
      ret.value = Randomize();
      break;
    }
    case RandOp::SetSeed: {
      if (vals.top().env) return false;
      seed_ = static_cast<unsigned int>(vals.top().value);
      vals.pop();
      ret.value = seed_;
      break;
    }
    case RandOp::GetSeed: {
      ret.value = seed_;
      break;
    }
    case RandOp::Next: {
      ret.value = std::rand();
      break;
    }
    case RandOp::Range: {
      // get left bound
      if (vals.top().env) return false;
      int l = vals.top().value;
      vals.pop();
      // get right bound
      if (vals.top().env) return false;
      int r = vals.top().value;
      vals.pop();
      // calculate the result
      ret.value = std::rand() % (r - l + 1) + l;
      break;
    }
    default: {
      assert(false);
      return false;
    }
  }
  ret.env = nullptr;
  return true;
}
