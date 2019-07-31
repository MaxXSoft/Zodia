#ifndef ZODIA_SCRIPT_RUNTIME_TIMER_H_
#define ZODIA_SCRIPT_RUNTIME_TIMER_H_

#include <string>
#include <unordered_map>
#include <functional>

#include "script/runtime.h"
#include "core/timer.h"

class TimerRuntime : public RuntimeBase {
 public:
  TimerRuntime();

  // getters
  bool is_callable() const override { return !op_.empty(); }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class TimerOp {
    Create, Remove, Reset, Check,
  };
  
  using PoolIter = std::unordered_map<unsigned int, Timer>::iterator;
  using OpFunc = std::function<void(const PoolIter &)>;

  bool FindAndPerform(ionia::vm::VM::ValueStack &vals,
                      ionia::vm::Value &ret, OpFunc op);

  // next timer id
  unsigned int next_id_;
  // timer pool
  std::unordered_map<unsigned int, Timer> pool_;
  // operation name
  std::string op_;
  // hashmap of opcodes
  std::unordered_map<std::string, TimerOp> ops_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_TIMER_H_
