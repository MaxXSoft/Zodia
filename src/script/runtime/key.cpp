#include "script/runtime/key.h"

using namespace ionia::vm;

KeyRuntime::KeyRuntime() {
  // initialize hashmap
  key_map_["a"] = kKeyA; key_map_["b"] = kKeyB; key_map_["c"] = kKeyC;
  key_map_["d"] = kKeyD; key_map_["e"] = kKeyE; key_map_["f"] = kKeyF;
  key_map_["g"] = kKeyG; key_map_["h"] = kKeyH; key_map_["i"] = kKeyI;
  key_map_["j"] = kKeyJ; key_map_["k"] = kKeyK; key_map_["l"] = kKeyL;
  key_map_["m"] = kKeyM; key_map_["n"] = kKeyN; key_map_["o"] = kKeyO;
  key_map_["p"] = kKeyP; key_map_["q"] = kKeyQ; key_map_["r"] = kKeyR;
  key_map_["s"] = kKeyS; key_map_["t"] = kKeyT; key_map_["u"] = kKeyU;
  key_map_["v"] = kKeyV; key_map_["w"] = kKeyW; key_map_["x"] = kKeyX;
  key_map_["y"] = kKeyY; key_map_["z"] = kKeyZ;
  key_map_["0"] = kKey0; key_map_["1"] = kKey1; key_map_["2"] = kKey2;
  key_map_["3"] = kKey3; key_map_["4"] = kKey4; key_map_["5"] = kKey5;
  key_map_["6"] = kKey6; key_map_["7"] = kKey7; key_map_["8"] = kKey8;
  key_map_["9"] = kKey9;
  key_map_["back"] = kKeyBackspace; key_map_["space"] = kKeySpace;
  key_map_["tab"] = kKeyTab;        key_map_["enter"] = kKeyEnter;
  key_map_["shift-l"] = kKeyShiftL; key_map_["shift-r"] = kKeyShiftR;
  key_map_["ctrl-l"] = kKeyCtrlL;   key_map_["ctrl-r"] = kKeyCtrlR;
  key_map_["alt-l"] = kKeyAltL;     key_map_["alt-r"] = kKeyAltR;
  key_map_["up"] = kKeyArrowU;      key_map_["down"] = kKeyArrowD;
  key_map_["left"] = kKeyArrowL;    key_map_["right"] = kKeyArrowR;
}

RuntimeRef KeyRuntime::GetChild(const std::string &name) {
  return nullptr;
}

RuntimeRef KeyRuntime::GetElement(const std::string &name) {
  if (key_.empty()) {
    key_ = name;
    return this;
  }
  else {
    return nullptr;
  }
}

bool KeyRuntime::Handler(VM::ValueStack &vals, Value &ret) {
  // get 'KeyName' by name
  auto it = key_map_.find(key_);
  if (it == key_map_.end()) return false;
  key_.clear();
  // get parameter
  if (vals.top().env) return false;
  KeyStatus status = vals.top().value;
  vals.pop();
  status |= vals.top().value << 32ULL;
  vals.pop();
  // check if key was pressed
  ret.env = nullptr;
  ret.value = KEY_PRESSED(status, it->second);
  return true;
}
