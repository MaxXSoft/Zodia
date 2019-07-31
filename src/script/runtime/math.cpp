#include "script/runtime/math.h"

#include <cmath>
#include <limits>
#include <cassert>

using namespace ionia::vm;

// some helper macros
#define RANGE_INT(v, l, r)                       \
  (static_cast<int>(v) >= static_cast<int>(l) && \
   static_cast<int>(v) <= static_cast<int>(r))

namespace {

// constants
constexpr float kPi = 3.14159265358979323846264338327950288;
constexpr float kPi2 = 1.57079632679489661923132169163975144;
constexpr float kPi4 = 0.785398163397448309615660845819875721;
constexpr float kE = 2.71828182845904523536028747135266250;

template <typename To, typename From>
inline To ToType(From value) {
  return *reinterpret_cast<To *>(&value);
}

inline bool FloatEqual(float a, float b) {
  auto abs_a = std::abs(a), abs_b = std::abs(b);
  return std::abs(a - b) <= (abs_a < abs_b ? abs_b : abs_a) *
                                std::numeric_limits<float>::epsilon();
}

}  // namespace

bool MathRuntime::MethodHandler(MethodType type, VM::ValueStack &vals,
                                Value &ret) {
  using Type = MethodType;
  // get operands
  int lhs, rhs;
  if (RANGE_INT(type, Type::Float, Type::Round) ||
      RANGE_INT(type, Type::Sqrt, Type::IAbs) ||
      RANGE_INT(type, Type::Exp, Type::ATanH)) {
    // unray operators
    if (vals.top().env) return false;
    lhs = vals.top().value;
    vals.pop();
  }
  else if (!(RANGE_INT(type, Type::Pi, Type::NaN))) {
    // binary operators
    if (vals.top().env) return false;
    rhs = vals.top().value;
    vals.pop();
  }
  // convert to float
  float f1 = ToType<float>(lhs), f2 = ToType<float>(rhs);
  // perform operations
  int ans;
  switch (type) {
    case Type::Pi: ans = ToType<int>(kPi); break;
    case Type::Pi2: ans = ToType<int>(kPi2); break;
    case Type::Pi4: ans = ToType<int>(kPi4); break;
    case Type::E: ans = ToType<int>(kE); break;
    case Type::Inf: ans = ToType<int>(INFINITY); break;
    case Type::NaN: ans = ToType<int>(NAN); break;
    case Type::Float: ans = ToType<int, float>(lhs); break;
    case Type::Int: ans = static_cast<int>(f1); break;
    case Type::Rad: ans = ToType<int>(f1 * kPi / 180); break;
    case Type::Deg: ans = ToType<int>(f1 * 180 / kPi); break;
    case Type::Ceil: ans = ToType<int>(std::ceil(f1)); break;
    case Type::Floor: ans = ToType<int>(std::floor(f1)); break;
    case Type::Trunc: ans = ToType<int>(std::trunc(f1)); break;
    case Type::Round: ans = ToType<int>(std::round(f1)); break;
    case Type::Eq: ans = FloatEqual(f1, f2); break;
    case Type::Neq: ans = !FloatEqual(f1, f2); break;
    case Type::Lt: ans = f1 < f2; break;
    case Type::Le: ans = f1 <= f2; break;
    case Type::Gt: ans = f1 > f2; break;
    case Type::Ge: ans = f1 >= f2; break;
    case Type::Add: ans = ToType<int>(f1 + f2); break;
    case Type::Sub: ans = ToType<int>(f1 - f2); break;
    case Type::Mul: ans = ToType<int>(f1 * f2); break;
    case Type::Div: ans = ToType<int>(f1 / f2); break;
    case Type::Mod: ans = ToType<int>(std::fmod(f1, f2)); break;
    case Type::Pow: ans = ToType<int>(std::pow(f1, f2)); break;
    case Type::Sqrt: ans = ToType<int>(std::sqrt(f1)); break;
    case Type::Abs: ans = ToType<int>(std::abs(f1)); break;
    case Type::IAbs: ans = std::abs(lhs); break;
    case Type::Max: ans = ToType<int>(std::max(f1, f2)); break;
    case Type::IMax: ans = std::max(lhs, rhs); break;
    case Type::Min: ans = ToType<int>(std::min(f1, f2)); break;
    case Type::IMin: ans = std::min(lhs, rhs); break;
    case Type::Exp: ans = ToType<int>(std::exp(f1)); break;
    case Type::Exp2: ans = ToType<int>(std::exp2(f1)); break;
    case Type::Log: ans = ToType<int>(std::log(f1)); break;
    case Type::Log10: ans = ToType<int>(std::log10(f1)); break;
    case Type::Log2: ans = ToType<int>(std::log2(f1)); break;
    case Type::Sin: ans = ToType<int>(std::sin(f1)); break;
    case Type::Cos: ans = ToType<int>(std::cos(f1)); break;
    case Type::Tan: ans = ToType<int>(std::tan(f1)); break;
    case Type::ASin: ans = ToType<int>(std::asin(f1)); break;
    case Type::ACos: ans = ToType<int>(std::acos(f1)); break;
    case Type::ATan: ans = ToType<int>(std::atan(f1)); break;
    case Type::SinH: ans = ToType<int>(std::sinh(f1)); break;
    case Type::CosH: ans = ToType<int>(std::cosh(f1)); break;
    case Type::TanH: ans = ToType<int>(std::tanh(f1)); break;
    case Type::ASinH: ans = ToType<int>(std::asinh(f1)); break;
    case Type::ACosH: ans = ToType<int>(std::acosh(f1)); break;
    case Type::ATanH: ans = ToType<int>(std::atanh(f1)); break;
    default: assert(false); return false;
  }
  // return the answer
  ret.env = nullptr;
  ret.value = ans;
  return true;
}

MathRuntime::MathRuntime() {
  // initialize hashmap
  methods_["pi?"] = MethodType::Pi;
  methods_["pi2?"] = MethodType::Pi2;
  methods_["pi4?"] = MethodType::Pi4;
  methods_["e?"] = MethodType::E;
  methods_["inf?"] = MethodType::Inf;
  methods_["nan?"] = MethodType::NaN;
  methods_["float"] = MethodType::Float;
  methods_["int"] = MethodType::Int;
  methods_["rad"] = MethodType::Rad;
  methods_["deg"] = MethodType::Deg;
  methods_["ceil"] = MethodType::Ceil;
  methods_["floor"] = MethodType::Floor;
  methods_["trunc"] = MethodType::Trunc;
  methods_["round"] = MethodType::Round;
  methods_["eq"] = MethodType::Eq;
  methods_["neq"] = MethodType::Neq;
  methods_["lt"] = MethodType::Lt;
  methods_["le"] = MethodType::Le;
  methods_["gt"] = MethodType::Gt;
  methods_["ge"] = MethodType::Ge;
  methods_["add"] = MethodType::Add;
  methods_["sub"] = MethodType::Sub;
  methods_["mul"] = MethodType::Mul;
  methods_["div"] = MethodType::Div;
  methods_["mod"] = MethodType::Mod;
  methods_["pow"] = MethodType::Pow;
  methods_["sqrt"] = MethodType::Sqrt;
  methods_["abs"] = MethodType::Abs;
  methods_["iabs"] = MethodType::IAbs;
  methods_["max"] = MethodType::Max;
  methods_["imax"] = MethodType::IMax;
  methods_["min"] = MethodType::Min;
  methods_["imin"] = MethodType::IMin;
  methods_["exp"] = MethodType::Exp;
  methods_["exp2"] = MethodType::Exp2;
  methods_["log"] = MethodType::Log;
  methods_["log10"] = MethodType::Log10;
  methods_["log2"] = MethodType::Log2;
  methods_["sin"] = MethodType::Sin;
  methods_["cos"] = MethodType::Cos;
  methods_["tan"] = MethodType::Tan;
  methods_["asin"] = MethodType::ASin;
  methods_["acos"] = MethodType::ACos;
  methods_["atan"] = MethodType::ATan;
  methods_["sinh"] = MethodType::SinH;
  methods_["cosh"] = MethodType::CosH;
  methods_["tanh"] = MethodType::TanH;
  methods_["asinh"] = MethodType::ASinH;
  methods_["acosh"] = MethodType::ACosH;
  methods_["atanh"] = MethodType::ATanH;
}

RuntimeRef MathRuntime::GetChild(const std::string &name) {
  if (name_.empty()) {
    name_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

RuntimeRef MathRuntime::GetElement(const std::string &name) {
  return nullptr;
}

bool MathRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // reset internal state
  name_.clear();
  // get specific method
  auto it = methods_.find(name_);
  if (it == methods_.end()) return false;
  // call method
  return MethodHandler(it->second, vals, ret);
}
