#include "script/host.h"

#include <sstream>
#include <fstream>
#include <iterator>
#include <cassert>

#include "ionia/src/front/lexer.h"
#include "ionia/src/front/parser.h"
#include "ionia/src/back/compiler/compiler.h"
#include "util/logger.h"

// runtime libraries
#include "script/runtime/math.h"
#include "script/runtime/key.h"
#include "script/runtime/rand.h"
#include "script/runtime/timer.h"
#include "script/runtime/sprite.h"

using namespace ionia;

void ScriptHost::InitRuntimes() {
  // TODO
  runtimes_["math"] = std::make_unique<MathRuntime>();
  runtimes_["key?"] = std::make_unique<KeyRuntime>();
  runtimes_["rand"] = std::make_unique<RandRuntime>();
  runtimes_["timer"] = std::make_unique<TimerRuntime>();
  runtimes_["sprite"] = std::make_unique<SpriteRuntime>(*this);
}

bool ScriptHost::SymErrorHandler(int id, const std::string &sym,
                                 vm::Value &val) {
  if (IsComplexPath(sym)) {
    // store the symbol name
    vms_[id].last_sym = sym;
    // return VM handler
    val = vms_[id].handler;
    return true;
  }
  return false;
}

bool ScriptHost::VMHandler(int id, vm::VM::ValueStack &vals,
                           vm::Value &ret) {
  const auto &sym = vms_[id].last_sym;
  // get runtime by path
  auto runtime = Parse(sym);
  if (!runtime) LOG_ERROR("invalid runtime library path");
  // call handler
  return runtime->Call(vals, ret);
}

std::size_t ScriptHost::PushBackNewVM() {
  using namespace std::placeholders;
  // create new VM instance
  vms_.emplace_back();
  auto &info = vms_.back();
  auto vm_id = vms_.size() - 1;
  // set handler of current instance
  auto hand = std::bind(&ScriptHost::SymErrorHandler, this, vm_id, _1, _2);
  info.vm.set_sym_error_handler(hand);
  return vm_id;
}

void ScriptHost::CallFunction(const std::string &name,
                              const std::vector<vm::Value> &args,
                              vm::Value &ret) {
  // check if is already cached
  auto it = func_id_map_.find(name);
  if (it != func_id_map_.end()) {
    auto success = vms_[it->second].vm.CallFunction(name, args, ret);
    assert(success);
  }
  else {
    // find out the correct VM id and store to cache
    for (int i = 0; i < vms_.size(); ++i) {
      if (vms_[i].vm.CallFunction(name, args, ret)) {
        func_id_map_[name] = i;
        return;
      }
    }
    // function not found
    LOG_ERROR("invalid script function call");
  }
}

void ScriptHost::AddInstance(const std::string &file) {
  // read file to buffer
  std::ifstream ifs(file, std::ios::binary);
  ifs >> std::noskipws;
  std::vector<std::uint8_t> buff((std::istreambuf_iterator<char>(ifs)),
                                 std::istreambuf_iterator<char>());
  // load script from buffer
  AddInstance(buff);
}

void ScriptHost::AddInstance(const std::vector<std::uint8_t> &buffer) {
  using namespace std::placeholders;
  // create new VM and get info
  auto id = PushBackNewVM();
  auto &info = vms_[id];
  auto &vm = info.vm;
  // try to load as bytecode
  if (!vm.LoadProgram(buffer)) {
    // compile and try again
    auto buff = buffer;
    buff.push_back('\0');
    buff = CompileSource(reinterpret_cast<char *>(buff.data()));
    if (!vm.LoadProgram(buff)) LOG_ERROR("invalid script");
    // set up external function
    auto ext_fun = std::bind(&ScriptHost::VMHandler, this, id, _1, _2);
    vm.RegisterAnonFunc(ext_fun, info.handler);
    // initialize VM instance
    if (!vm.Run()) LOG_ERROR("failed to initialize VM instance");
  }
}

std::vector<std::uint8_t> ScriptHost::CompileSource(
    const std::string &source) {
  // initialize input stream
  std::istringstream iss(source);
  // initialize lexer, parser and compiler
  Lexer lexer(iss);
  Parser parser(lexer);
  Compiler comp;
  // get ASTs
  while (auto ast = parser.ParseNext()) {
    comp.CompileNext(ast);
  }
  if (lexer.error_num() || parser.error_num()) LOG_ERROR("syntax error");
  // generate bytecode
  return comp.GenerateBytecode();
}

void ScriptHost::RegisterScene(const std::string &name) {
  assert(sprite_map_.find(name) == sprite_map_.end());
  sprite_map_[name] = {};
}

void ScriptHost::RegisterSprite(const std::string &scene,
                                const std::string &name, int layer_id) {
  auto it = sprite_map_.find(scene);
  if (it == sprite_map_.end()) LOG_ERROR("scene does not exist");
  auto ret = it->second.insert({name, layer_id});
  if (!ret.second) LOG_ERROR("sprite id conflicted");
}

void ScriptHost::CallResetHandler(const std::string &name) {
  vm::Value ret;
  CallFunction(name, {}, ret);
}

void ScriptHost::CallBeginHandler(const std::string &name, KeyStatus key) {
  vm::Value ret;
  // TODO: fix this compromise method
  auto k1 = static_cast<int>(key & 0xffffffff);
  auto k2 = static_cast<int>(key >> 32);
  CallFunction(name, {{k1}, {k2}}, ret);
}
