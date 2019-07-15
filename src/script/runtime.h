#ifndef ZODIA_SCRIPT_RUNTIME_H_
#define ZODIA_SCRIPT_RUNTIME_H_

#include <string>
#include <memory>

#include "ionia/src/vm/vm.h"

// interface of runtime library in script host
class RuntimeInterface {
 public:
  // handler of library
  virtual bool Handler(const std::string &path,
                       ionia::vm::VM::ValueStack &vals,
                       ionia::vm::Value &ret) = 0;
  // get library name
  virtual const std::string &name() const = 0;
};

using RuntimePtr = std::unique_ptr<RuntimeInterface>;

#endif  // ZODIA_SCRIPT_RUNTIME_H_
