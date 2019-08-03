#include "script/runtime.h"

#include <algorithm>

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
    int start = path.front() == '.' ? 1 : 0;
    // get the position of '.' or '['
    auto pos_dot = path.find('.', start);
    auto pos_brk = path.find('[', start);
    auto pos = std::min(pos_dot, pos_brk);
    // get reference of child
    ref = GetChild(path.substr(start, pos));
    if (pos != std::string::npos) rest = path.substr(pos);
  }
  return ref ? ref->Parse(rest) : nullptr;
}

bool RuntimeBase::Call(VM::ValueStack &vals, Value &ret) {
  if (!is_callable()) return false;
  return Handler(vals, ret);
}
