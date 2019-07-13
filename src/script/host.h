#ifndef ZODIA_SCRIPT_HOST_H_
#define ZODIA_SCRIPT_HOST_H_

#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

#include "ionia/src/vm/vm.h"
#include "scene/sceneman.h"

class ScriptHost {
 public:
  ScriptHost(SceneManager &scene_man) : scene_man_(scene_man) { Clear(); }

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

  // call the function in script by name
  void CallFunction(const std::string &name);
  // register scene with name by scene id
  void RegisterScene(const std::string &name, int id);
  // register sprite in scene
  void RegisterSprite(const std::string &name, const std::string &scene,
                      int layer_id);

 private:
  // information of sprite in GameScene
  using SpriteInfo = std::unordered_map<std::string, int>;
  // information of scene in SceneManager
  struct SceneInfo {
    int id;
    SpriteInfo sprite_info;
  };

  // reference of scene manager in game window
  SceneManager &scene_man_;
  // all VM instances
  std::vector<ionia::vm::VM> vms_;
  // cache for mapping function name to VM id
  std::unordered_map<std::string, int> func_id_map_;
  // information of scenes & sprites
  std::unordered_map<std::string, SceneInfo> scene_map_;
};

#endif  // ZODIA_SCRIPT_HOST_H_
