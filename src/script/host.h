#ifndef ZODIA_SCRIPT_HOST_H_
#define ZODIA_SCRIPT_HOST_H_

#include <vector>
#include <string>
#include <map>
#include <cstdint>

#include "ionia/src/vm/vm.h"

class ScriptHost {
 public:
  ScriptHost() { Clear(); }

  // clear all VM instances
  void Clear() {
    vms_.clear();
    func_id_map_.clear();
  }

  // add a new VM instance (from file)
  void AddInstance(const std::string &file);
  // add a new VM instance (from memory)
  void AddInstance(const std::vector<std::uint8_t> &buffer);
  // compile source code to bytecode
  std::vector<std::uint8_t> CompileSource(const std::string &source);

 private:
  std::vector<ionia::vm::VM> vms_;
  std::map<std::string, int> func_id_map_;
};

#endif  // ZODIA_SCRIPT_HOST_H_
