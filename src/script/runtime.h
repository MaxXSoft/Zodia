#ifndef ZODIA_SCRIPT_RUNTIME_H_
#define ZODIA_SCRIPT_RUNTIME_H_

#include <string>
#include <memory>

#include "ionia/src/vm/vm.h"

// type definitions
class RuntimeBase;
using RuntimePtr = std::unique_ptr<RuntimeBase>;
using RuntimeRef = RuntimeBase *;

// base class of runtime library in script host
class RuntimeBase {
 public:
  // check if is a complex runtime path (including '.' or '[*]')
  static bool IsComplexPath(const std::string &path) {
    return path.find('.') != std::string::npos ||
           (path.find('[') != std::string::npos &&
            path.find(']') != std::string::npos);
  }

  // parse the path and return a reference of next runtime
  RuntimeRef Parse(const std::string &path);
  // call the handler of current runtime
  bool Call(ionia::vm::VM::ValueStack &vals, ionia::vm::Value &ret);

  // getters
  virtual bool is_callable() const = 0;

 protected:
  // get child object (like '.id') by name
  virtual RuntimeRef GetChild(const std::string &name) = 0;
  // get element object (like '[id]') by name
  virtual RuntimeRef GetElement(const std::string &name) = 0;
  // handler of current runtime
  virtual bool Handler(ionia::vm::VM::ValueStack &vals,
                       ionia::vm::Value &ret) = 0;
};

#endif  // ZODIA_SCRIPT_RUNTIME_H_
