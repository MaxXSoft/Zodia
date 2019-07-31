#ifndef ZODIA_SCRIPT_RUNTIME_RAND_H_
#define ZODIA_SCRIPT_RUNTIME_RAND_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"

class RandRuntime : public RuntimeBase {
 public:
  RandRuntime();

  void ResetState() override { op_.clear(); }

  // getters
  bool is_callable() const override { return !op_.empty(); }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class RandOp {
    Randomize, SetSeed, GetSeed, Next, Range,
  };

  int Randomize();

  // seed of rand generator
  unsigned int seed_;
  // operation name
  std::string op_;
  // hashmap of opcodes
  std::unordered_map<std::string, RandOp> ops_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_RAND_H_
