#ifndef ZODIA_SCRIPT_RUNTIME_HELPER_BOUNDING_H_
#define ZODIA_SCRIPT_RUNTIME_HELPER_BOUNDING_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"
#include "script/runtime/sprite.h"

class BoundingRuntime : public RuntimeBase {
 public:
  BoundingRuntime(const SpriteRuntime &sprite_rt) : sprite_rt_(sprite_rt) {
    InitOpMap();
  }

  void ResetState() override { op_.clear(); };

  // getters
  bool is_callable() const override { return true; }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class BoundingOp {
    Call, SetX, GetX, SetY, GetY,
    SetWidth, GetWidth, SetHeight, GetHeight,
  };

  // initialize opcode map
  void InitOpMap();

  // reference of 'SpriteRuntime'
  const SpriteRuntime &sprite_rt_;
  // operation name
  std::string op_;
  // hashmap of opcodes
  std::unordered_map<std::string, BoundingOp> ops_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_HELPER_BOUNDING_H_
