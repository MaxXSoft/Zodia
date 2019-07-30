#ifndef ZODIA_SCRIPT_RUNTIME_MATH_H_
#define ZODIA_SCRIPT_RUNTIME_MATH_H_

#include <string>
#include <unordered_map>
#include <functional>

#include "script/runtime.h"

class MathRuntime : public RuntimeBase {
 public:
  MathRuntime();

  // getters
  bool is_callable() const override { return !name_.empty(); }

 protected:
  RuntimeRef GetChild(const std::string &name) override;
  RuntimeRef GetElement(const std::string &name) override;
  bool Handler(ionia::vm::VM::ValueStack &vals,
               ionia::vm::Value &ret) override;

 private:
  enum class MethodType {
    // constants
    Pi, Pi2, Pi4, E, Inf, NaN,
    // converters
    Float, Int, Rad, Deg,
    Ceil, Floor, Trunc, Round,
    // basic operations
    Eq, Neq, Lt, Le, Gt, Ge,
    Add, Sub, Mul, Div, Mod, Pow, Sqrt,
    Abs, IAbs, Max, IMax, Min, IMin,
    // exponential functions
    Exp, Exp2, Log, Log10, Log2,
    // trigonometric functions
    Sin, Cos, Tan, ASin, ACos, ATan,
    SinH, CosH, TanH, ASinH, ACosH, ATanH,
  };

  static bool MethodHandler(MethodType type,
                            ionia::vm::VM::ValueStack &vals,
                            ionia::vm::Value &ret);

  // method name
  std::string name_;
  // hashmap of method types
  std::unordered_map<std::string, MethodType> methods_;
};

#endif  // ZODIA_SCRIPT_RUNTIME_MATH_H_
