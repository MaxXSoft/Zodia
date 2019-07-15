#ifndef ZODIA_SCRIPT_HOST_H_
#define ZODIA_SCRIPT_HOST_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

#include "ionia/src/vm/vm.h"
#include "scene/sceneman.h"
#include "define/key.h"

class ScriptHost {
 public:
  ScriptHost(SceneManager &scene_man) : scene_man_(scene_man) { Clear(); }

  // clear all stored information
  void Clear() {
    vms_.clear();
    func_id_map_.clear();
    scene_map_.clear();
  }

  // add a new VM instance (from file)
  void AddInstance(const std::string &file);
  // add a new VM instance (from memory)
  void AddInstance(const std::vector<std::uint8_t> &buffer);
  // compile source code to bytecode
  std::vector<std::uint8_t> CompileSource(const std::string &source);

  // register scene with name by scene id
  void RegisterScene(const std::string &name, int id);
  // register sprite in scene
  void RegisterSprite(const std::string &name, const std::string &scene,
                      int layer_id);
  // call handler of 'Reset' event in game scene
  void CallResetHandler(const std::string &name);
  // call handler of 'FrameBegin' event in game scene
  void CallBeginHandler(const std::string &name, KeyStatus key);

 private:
  // information of sprite in GameScene
  using SpriteInfo = std::unordered_map<std::string, int>;
  // information of scene in SceneManager
  struct SceneInfo {
    int id;
    SpriteInfo sprite_info;
  };
  // information of VM instance
  struct VMInfo {
    ionia::vm::VM vm;
    ionia::vm::Value handler;
  };

  // push a new VM instance back to 'vms_'
  ionia::vm::VM &PushBackNewVM();
  // symbol error handler of all VM instances
  bool SymErrorHandler(int id, const std::string &sym,
                       ionia::vm::Value &val);
  // external function in all VM instances
  bool VMHandler(ionia::vm::VM::ValueStack &vals, ionia::vm::Value &ret);
  // call the function in script by name
  void CallFunction(const std::string &name,
                    const std::vector<ionia::vm::Value> &args,
                    ionia::vm::Value &ret);

  // reference of scene manager in game window
  SceneManager &scene_man_;
  // all VM instances
  std::vector<VMInfo> vms_;
  // cache for mapping function name to VM id
  std::unordered_map<std::string, int> func_id_map_;
  // information of scenes & sprites
  std::unordered_map<std::string, SceneInfo> scene_map_;
};

#endif  // ZODIA_SCRIPT_HOST_H_
