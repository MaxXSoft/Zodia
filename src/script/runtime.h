#ifndef ZODIA_SCRIPT_RUNTIME_H_
#define ZODIA_SCRIPT_RUNTIME_H_

#include <string>
#include <memory>

#include "ionia/src/vm/vm.h"

// forward declaration of script host
class ScriptHost;

// base class of runtime library in script host
class RuntimeBase {
 public:
  RuntimeBase(const ScriptHost &host) : host_(host) {}

  // handler of library
  virtual bool Handler(const std::string &path,
                       ionia::vm::VM::ValueStack &vals,
                       ionia::vm::Value &ret) = 0;

  // getters
  // get library name
  virtual const std::string &name() const = 0;

 protected:
  const ScriptHost &host() const { return host_; }

 private:
  const ScriptHost &host_;
};

using RuntimePtr = std::unique_ptr<RuntimeBase>;

#endif  // ZODIA_SCRIPT_RUNTIME_H_
