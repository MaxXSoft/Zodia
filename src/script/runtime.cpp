#include "script/runtime.h"

#include "util/logger.h"

using namespace ionia::vm;

RuntimeRef RuntimeBase::Parse(const std::string &path) {
  // check if path is empty
  if (path.empty()) return this;
  // parse the path
  RuntimeRef ref = nullptr;
  std::string rest;
  if (path.front() == '[') {
    // check ']'
    auto pos = path.find(']');
    if (pos == std::string::npos) LOG_ERROR("invalid path");
    // get reference of element
    ref = GetElement(path.substr(1, pos - 1));
    rest = path.substr(pos + 1);
  }
  else {
    // get the position of '.'
    auto pos = path.find('.');
    // get reference of child
    ref = GetChild(path.substr(0, pos));
    if (pos != std::string::npos) rest = path.substr(pos + 1);
  }
  return ref ? ref->Parse(rest) : nullptr;
}

bool RuntimeBase::Call(VM::ValueStack &vals, Value &ret) {
  if (!is_callable()) return false;
  return Handler(vals, ret);
}
