#ifndef ZODIA_SCRIPT_RUNTIME_KEY_H_
#define ZODIA_SCRIPT_RUNTIME_KEY_H_

#include <string>
#include <unordered_map>

#include "script/runtime.h"
#include "define/key.h"

class KeyRuntime : public RuntimeBase {
 public:
  KeyRuntime();

  void ResetState() override { key_.clear(); }

  // getters
  bool is_callable() const override { return !key_.empty(); }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  // key name
  std::string key_;
  // hashmap of keys
  std::unordered_map<std::string, KeyName> key_map_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_KEY_H_
