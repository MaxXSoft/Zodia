#ifndef ZODIA_SCRIPT_RUNTIME_SPRITE_H_
#define ZODIA_SCRIPT_RUNTIME_SPRITE_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"
#include "script/host.h"
#include "sprite/sprite.h"

class SpriteRuntime : public RuntimeBase {
 public:
  SpriteRuntime(const ScriptHost &host) : host_(host) { InitOpMap(); }

  void ResetState() override {
    sprite_ = nullptr;
    op_.clear();
  }

  // getters
  bool is_callable() const override { return sprite_ && !op_.empty(); }
  const SpritePtr &sprite() const { return sprite_; }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class SpriteOp {
    // setters & getters
    SetX, GetX, SetY, GetY, SetWidth, GetWidth,
    SetHeight, GetHeight, SetVisible, GetVisible,
    // methods
    Remove,
    // setter & getter of 'Number'
    SetNumber, GetNumber,
  };

  // initialize opcode map
  void InitOpMap();

  // parent host
  const ScriptHost &host_;
  // selected sprite
  SpritePtr sprite_;
  // operation name
  std::string op_;
  // hashmap of opcodes
  std::unordered_map<std::string, SpriteOp> ops_;
  // named children
  std::unordered_map<std::string, RuntimePtr> children_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_SPRITE_H_
