#include "game/mapparser.h"

#include <fstream>
#include <iterator>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "version.h"
#include "game/generalscene.h"
#include "sprite/background.h"
#include "sprite/number.h"
#include "sprite/group.h"
#include "util/logger.h"
#include "util/file.h"
#include "util/base64.h"

using namespace rapidxml;

namespace {

inline bool IsValidMap(const std::string &xmlns) {
  // check prefix
  std::string ns = "http://maxxsoft.net/schemas/zodia/v";
  if (xmlns.substr(0, ns.size()) != ns) return false;
  // read version
  auto version = xmlns.substr(ns.size());
  int major, minor, patch;
  std::sscanf(version.c_str(), "%d.%d.%d", &major, &minor, &patch);
  // check version
  if (major > APP_VERSION_MAJOR) {
    return false;
  }
  else if (major == APP_VERSION_MAJOR) {
    if (minor > APP_VERSION_MINOR) {
      return false;
    }
    else if (minor == APP_VERSION_MINOR) {
      if (patch > APP_VERSION_PATCH) return false;
    }
  }
  return true;
}

inline int GetInt(const char *str) {
  return std::atoi(str);
}

inline bool GetBool(const char *str) {
  return !std::strcmp(str, "1") || !std::strcmp(str, "true");
}

inline std::pair<int, int> GetIntPair(const char *str) {
  int x, y;
  std::sscanf(str, "%d,%d", &x, &y);
  return {x, y};
}

inline SDLRect GetRect(const char *str) {
  SDLRect rect;
  std::sscanf(str, "%d,%d,%d,%d", &rect.x, &rect.y, &rect.w, &rect.h);
  return rect;
}

inline Background::Arrange GetArrange(const char *str) {
  using Arrange = Background::Arrange;
  if (!std::strcmp(str, "fill")) return Arrange::Fill;
  if (!std::strcmp(str, "tile")) return Arrange::Tile;
  if (!std::strcmp(str, "stretch")) return Arrange::Stretch;
  LOG_ERROR("invalid arrange");
}

inline std::string GetPath(const std::string &file, const char *base) {
  std::string path = GetBasePath(file);
  if (base) {
    if (IsAbsPath(base)) {
      path = NormalizePath(base);
    }
    else {
      path += NormalizePath(base);
    }
  }
  return path;
}

}  // namespace

void MapParser::ReadFile() {
  // read stream into vector
  std::ifstream ifs(file_);
  std::vector<char> buffer((std::istreambuf_iterator<char>(ifs)),
                           std::istreambuf_iterator<char>());
  buffer.push_back('\0');
  // parse file
  doc_.parse<0>(buffer.data());
  // get root node
  root_ = doc_.first_node("game");
  if (!root_) LOG_ERROR("invalid map file, root node not found");
}

void MapParser::ParseResources(xml_node<> *node) {
  // get base path
  auto base = node->first_attribute("base");
  auto path = GetPath(file_, base ? base->value() : nullptr);
  // traversal all images
  for (auto i = node->first_node(); i; i->next_sibling()) {
    ParseImage(i, path);
  }
}

void MapParser::ParseImage(xml_node<> *node, const std::string &base) {
  // get id
  auto id = node->first_attribute("id");
  if (!id) LOG_ERROR("expected resource identifier");
  // load image
  if (!std::strcmp(node->name(), "image")) {
    auto source = node->first_attribute("source");
    if (!source) LOG_ERROR("expected source");
    window_.res_man().LoadImage(id->value(), base + source->value());
  }
  else if (!std::strcmp(node->name(), "raw")) {
    auto buff = ParseRaw(node);
    window_.res_man().LoadImage(id->value(), buff);
  }
  else {
    LOG_ERROR("unknown image node name");
  }
}

void MapParser::ParseScripts(xml_node<> *node) {
  // get base path
  auto base = node->first_attribute("base");
  auto path = GetPath(file_, base ? base->value() : nullptr);
  // traversal all images
  for (auto i = node->first_node(); i; i->next_sibling()) {
    ParseScript(i, path);
  }
}

void MapParser::ParseScript(xml_node<> *node, const std::string &base) {
  if (!std::strcmp(node->name(), "script")) {
    // add script from file
    auto source = node->first_attribute("source");
    if (!source) LOG_ERROR("expected source");
    host_.AddInstance(source->value());
  }
  else if (!std::strcmp(node->name(), "raw")) {
    // add script from raw data
    auto buff = ParseRaw(node);
    host_.AddInstance(buff);
  }
  else {
    LOG_ERROR("unknown script node name");
  }
}

void MapParser::ParseScenes(xml_node<> *node) {
  // traversal all scene nodes
  SceneNum scene_num;
  for (auto i = node->first_node("scene"); i; i = i->next_sibling()) {
    ParseScene(i, scene_num);
  }
  // set default scene
  auto def_id = node->first_attribute("default");
  if (!def_id) {
    auto it = scene_num.find(def_id->value());
    if (it == scene_num.end()) {
      LOG_ERROR("invalid id of default scene");
    }
    else {
      window_.scene_man().SwitchScene(it->second);
    }
  }
}

void MapParser::ParseScene(xml_node<> *node, SceneNum &scene_num) {
  // get attributes
  auto id = node->first_attribute("id");
  if (!id) LOG_ERROR("scene must have id");
  auto reset = node->first_attribute("reset");
  auto frame_begin = node->first_attribute("frame-begin");
  // traversal all layers
  Layers layers;
  for (auto i = node->first_node("layer"); i; i = i->next_sibling()) {
    ParseLayer(i, layers);
  }
  // create game scene
  auto scene = std::make_shared<GeneralScene>(host_, layers.size());
  // add layers to scene
  int order = 0;
  for (const auto &i : layers) {
    for (const auto &it : i.second) {
      scene->GetScene()->AddSprite(order, it.first, it.second);
    }
    ++order;
  }
  // set handlers
  if (reset) scene->set_reset_handler(reset->value());
  if (frame_begin) scene->set_frame_begin_handler(frame_begin->value());
  // add scene to manager
  auto num = window_.scene_man().AddScene(scene);
  auto ret = scene_num.insert({id->value(), num});
  if (!ret.second) LOG_ERROR("scene id conflicted");
}

void MapParser::ParseLayer(xml_node<> *node, Layers &layers) {
  // get order
  auto order = node->first_attribute("order");
  int order_val;
  if (order) {
    order_val = GetInt(order->value());
  }
  else {
    order_val = layers.empty() ? 0 : layers.rbegin()->first + 1;
  }
  // traversal all sprites
  for (auto i = node->first_node(); i; i->next_sibling()) {
    ParseSprite(i, layers[order_val]);
  }
}

void MapParser::ParseSprite(xml_node<> *node, Sprites &sprites) {
  // get general attributes
  auto id = node->first_attribute("id");
  if (!id) LOG_ERROR("sprite must have id");
  auto visible = node->first_attribute("visible");
  // parse the rest by node name
  SpritePtr sprite;
  if (!std::strcmp(node->name(), "sprite")) {
    sprite = ParseSprite(node);
  }
  else if (!std::strcmp(node->name(), "background")) {
    sprite = ParseBackground(node);
  }
  else if (!std::strcmp(node->name(), "number")) {
    sprite = ParseNumber(node);
  }
  else if (!std::strcmp(node->name(), "group")) {
    sprite = ParseGroup(node);
  }
  else {
    LOG_ERROR("unknown sprite node name");
  }
  // add to sprites
  if (visible) sprite->set_visible(GetBool(visible->value()));
  auto ret = sprites.insert({id->value(), sprite});
  if (!ret.second) LOG_ERROR("sprite id conflicted");
}

MapParser::Bytes MapParser::ParseRaw(xml_node<> *node) {
  // get type
  auto type = node->first_attribute("type");
  if (!type) LOG_ERROR("<raw> node must have type");
  // check type
  std::string ty = type->value();
  if (ty.substr(0, 5) == "image" || ty == "ionia/bytecode") {
    // parse as base64 encoded binary
    return DecodeBase64(node->value());
  }
  else {
    // parse as string
    return Bytes(node->value(),
                 node->value() + std::strlen(node->value()) + 1);
  }
}

SpritePtr MapParser::ParseSprite(xml_node<> *node) {
  SpritePtr sprite;
  // get attributes
  auto texture = node->first_attribute("texture");
  auto position = node->first_attribute("position");
  auto clip = node->first_attribute("clip");
  auto bounding = node->first_attribute("bounding");
  // parse attributes
  SDLTexturePtr text;
  std::pair<int, int> pos;
  SDLRect cl, bd;
  if (texture) text = window_.res_man().GetImage(texture->value());
  if (position) pos = GetIntPair(position->value());
  if (clip) cl = GetRect(clip->value());
  if (bounding) bd = GetRect(bounding->value());
  // create sprite
  if (!texture) {
    sprite = std::make_shared<Sprite>();
  }
  else if (!clip) {
    sprite = std::make_shared<Sprite>(text);
  }
  else {
    sprite = std::make_shared<Sprite>(text, cl.x, cl.y, cl.w, cl.h);
  }
  if (position) {
    sprite->set_x(pos.first);
    sprite->set_y(pos.second);
  }
  if (bounding) sprite->set_bounding(bd);
  return sprite;
}

SpritePtr MapParser::ParseBackground(xml_node<> *node) {
  SpritePtr sprite;
  // get attributes
  auto texture = node->first_attribute("texture");
  if (!texture) LOG_ERROR("expected texture");
  auto clip = node->first_attribute("clip");
  auto arrange = node->first_attribute("arrange");
  // parse attributes
  SDLRect cl;
  if (clip) cl = GetRect(clip->value());
  auto text = window_.res_man().GetImage(texture->value());
  auto arr = arrange ? GetArrange(arrange->value()) :
                       Background::Arrange::Fill;
  // create background
  if (!clip) {
    sprite = std::make_shared<Background>(text, arr);
  }
  else {
    sprite = std::make_shared<Background>(text, cl.x, cl.y,
                                          cl.w, cl.h, arr);
  }
  return sprite;
}

SpritePtr MapParser::ParseNumber(xml_node<> *node) {
  std::shared_ptr<Number> sprite;
  // get attributes
  auto texture = node->first_attribute("texture");
  if (!texture) LOG_ERROR("expected texture");
  auto clip = node->first_attribute("clip");
  auto number = node->first_attribute("number");
  // parse attributes
  SDLRect cl;
  if (clip) cl = GetRect(clip->value());
  auto text = window_.res_man().GetImage(texture->value());
  auto num = number ? GetInt(number->value()) : 0;
  // create number
  if (!clip) {
    sprite = std::make_shared<Number>(text);
  }
  else {
    sprite = std::make_shared<Number>(text, cl.x, cl.y, cl.w, cl.h);
  }
  sprite->set_number(num);
  return sprite;
}

SpritePtr MapParser::ParseGroup(xml_node<> *node) {
  // traversal all sprites
  Sprites sprites;
  for (auto i = node->first_node(); i; i->next_sibling()) {
    ParseSprite(i, sprites);
  }
  // create group
  auto sprite = std::make_shared<Group>();
  for (const auto &i : sprites) {
    sprite->AddSprite(i.second);
  }
  return sprite;
}

void MapParser::Reset() {
  // reset game window & script host
  window_.res_man().Clear();
  window_.scene_man().Clear();
  host_.Clear();
  // read map file
  ReadFile();
}

void MapParser::Parse() {
  // check map version
  auto xmlns = root_->first_attribute("xmlns");
  if (!xmlns) LOG_ERROR("invalid map file");
  if (!IsValidMap(xmlns->value())) {
    LOG_ERROR("trying to load a newer version map file");
  }
  // set window title & size
  auto title = root_->first_attribute("title");
  if (title) window_.set_title(title->value());
  auto size = root_->first_attribute("size");
  if (size) {
    auto sz = GetIntPair(size->value());
    window_.set_size(sz.first, sz.second);
  }
  // parse resources
  auto resources = root_->first_node("resources");
  if (!resources) LOG_ERROR("<resources> not found");
  ParseResources(resources);
  // parse scripts
  auto scripts = root_->first_node("scripts");
  if (!scripts) LOG_ERROR("<scripts> not found");
  ParseScripts(scripts);
  // parse scenes
  auto scenes = root_->first_node("scenes");
  if (!scenes) LOG_ERROR("<scenes> not found");
  ParseScenes(scenes);
}
