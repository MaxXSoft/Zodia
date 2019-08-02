#ifndef ZODIA_SCRIPT_RUNTIME_HELPER_COLLIDE_H_
#define ZODIA_SCRIPT_RUNTIME_HELPER_COLLIDE_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"
#include "script/runtime/sprite.h"
#include "script/host.h"
#include "sprite/sprite.h"

class CollideRuntime : public RuntimeBase {
 public:
  CollideRuntime(const SpriteRuntime &sprite_rt, const ScriptHost &host)
      : sprite_rt_(sprite_rt), host_(host) {}

  void ResetState() override { sprite_ = nullptr; };

  // getters
  bool is_callable() const override { return !!sprite_; }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  // reference of 'SpriteRuntime'
  const SpriteRuntime &sprite_rt_;
  // reference of script host
  const ScriptHost &host_;
  // selected sprite
  SpritePtr sprite_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_HELPER_COLLIDE_H_
