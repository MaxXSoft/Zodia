#ifndef ZODIA_MAP_MAPPARSER_H_
#define ZODIA_MAP_MAPPARSER_H_

#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>

#include "rapidxml/rapidxml.hpp"

#include "core/window.h"
#include "sprite/sprite.h"
#include "script/host.h"

class MapParser {
 public:
  MapParser(Window &window, ScriptHost &host, const std::string &file)
      : window_(window), host_(host), file_(file) {
    Reset();
  }

  // reset parser
  void Reset();
  // parse map and update window
  void Parse();

 private:
  using Bytes = std::vector<std::uint8_t>;
  using Sprites = std::unordered_map<std::string, SpritePtr>;
  using Layers = std::map<int, Sprites>;

  // read content of input stream to RapidXml
  void ReadFile();

  void ParseResources(rapidxml::xml_node<> *node);
  void ParseImage(rapidxml::xml_node<> *node, const std::string &base);
  void ParseScripts(rapidxml::xml_node<> *node);
  void ParseScript(rapidxml::xml_node<> *node, const std::string &base);
  void ParseScenes(rapidxml::xml_node<> *node);
  void ParseScene(rapidxml::xml_node<> *node);
  void ParseLayer(rapidxml::xml_node<> *node, Layers &layers);
  void ParseSprite(rapidxml::xml_node<> *node, Sprites &sprites);

  Bytes ParseRaw(rapidxml::xml_node<> *node);
  SpritePtr ParseSprite(rapidxml::xml_node<> *node);
  SpritePtr ParseBackground(rapidxml::xml_node<> *node);
  SpritePtr ParseNumber(rapidxml::xml_node<> *node);
  SpritePtr ParseGroup(rapidxml::xml_node<> *node);

  // game window & script host
  Window &window_;
  ScriptHost &host_;
  // path of '*.zodia' file
  std::string file_;
  // RapidXml stuffs
  rapidxml::xml_document<> doc_;
  rapidxml::xml_node<> *root_;
};

#endif  // ZODIA_MAP_MAPPARSER_H_
