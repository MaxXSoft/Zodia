#include "script/host.h"

#include <sstream>
#include <fstream>
#include <iterator>

#include "ionia/src/front/lexer.h"
#include "ionia/src/front/parser.h"
#include "ionia/src/back/compiler/compiler.h"
#include "util/logger.h"

using namespace ionia;

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
  // try to load as bytecode
  vms_.emplace_back();
  if (!vms_.back().LoadProgram(buffer)) {
    // compile and try again
    auto buff = buffer;
    buff.push_back('\0');
    buff = CompileSource(reinterpret_cast<char *>(buff.data()));
    if (!vms_.back().LoadProgram(buff)) LOG_ERROR("invalid script");
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

void ScriptHost::CallFunction(const std::string &name) {
  //
}

void ScriptHost::RegisterScene(const std::string &name, int id) {
  scene_map_[name] = {id, {}};
}

void ScriptHost::RegisterSprite(const std::string &name,
                                const std::string &scene, int layer_id) {
  auto it = scene_map_.find(scene);
  if (it == scene_map_.end()) LOG_ERROR("scene does not exist");
  auto ret = it->second.sprite_info.insert({name, layer_id});
  if (!ret.second) LOG_ERROR("sprite id conflicted");
}
