#include "script/runtime/timer.h"

#include <utility>
#include <cassert>

using namespace ionia::vm;

TimerRuntime::TimerRuntime() {
  next_id_ = 0;
  // initialize hashmap
  ops_["create"] = TimerOp::Create;
  ops_["remove"] = TimerOp::Remove;
  ops_["reset"] = TimerOp::Reset;
  ops_["check"] = TimerOp::Check;
}

bool TimerRuntime::FindAndPerform(VM::ValueStack &vals, Value &ret,
                                  OpFunc op) {
  // get specific timer id from stack
  if (vals.top().env) return false;
  auto id = static_cast<unsigned int>(vals.top().value);
  vals.pop();
  // find and perform
  auto it = pool_.find(id);
  if (it == pool_.end()) {
    ret.value = 0;
  }
  else {
    op(it);
    ret.value = 1;
  }
  return true;
}

RuntimeRef TimerRuntime::GetChild(const std::string &name) {
  if (op_.empty()) {
    op_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

RuntimeRef TimerRuntime::GetElement(const std::string &name) {
  return nullptr;
}

bool TimerRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get opcode by name
  auto it = ops_.find(op_);
  if (it == ops_.end()) return false;
  op_.clear();
  // perform operations
  switch (it->second) {
    case TimerOp::Create: {
      pool_.emplace(std::make_pair(next_id_, Timer()));
      ret.value = static_cast<int>(next_id_);
      ++next_id_;
      break;
    }
    case TimerOp::Remove: {
      if (!FindAndPerform(vals, ret, [this](const PoolIter &it) {
        pool_.erase(it);
      })) return false;
      break;
    }
    case TimerOp::Reset: {
      if (!FindAndPerform(vals, ret, [](const PoolIter &it) {
        it->second.Restart();
      })) return false;
      break;
    }
    case TimerOp::Check: {
      // get id & interval from stack
      if (vals.top().env) return false;
      auto id = static_cast<unsigned int>(vals.top().value);
      vals.pop();
      if (vals.top().env) return false;
      auto interval = static_cast<unsigned int>(vals.top().value);
      vals.pop();
      // find and check interval
      auto it = pool_.find(id);
      if (it == pool_.end()) {
        ret.value = -1;
      }
      else {
        ret.value = it->second.CheckInterval(interval);
      }
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
