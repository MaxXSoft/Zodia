#ifndef ZODIA_SCRIPT_RUNTIME_SCENE_H_
#define ZODIA_SCRIPT_RUNTIME_SCENE_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"
#include "script/host.h"

class SceneRuntime : public RuntimeBase {
 public:
  SceneRuntime(const ScriptHost &host) : host_(host) { InitOpMap(); }

  void ResetState() override {
    op_.clear();
    selected_.clear();
  }

  // getters
  bool is_callable() const override { return !op_.empty(); }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class SceneOp {
    Quit, Pause, Render, Switch,
  };

  // initialize opcode map
  void InitOpMap();

  // parent host
  const ScriptHost &host_;
  // operation name & selected scene name
  std::string op_, selected_;
  // hashmap of opcodes
  std::unordered_map<std::string, SceneOp> ops_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_SCENE_H_
