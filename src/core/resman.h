#ifndef ZODIA_CORE_RESMAN_H_
#define ZODIA_CORE_RESMAN_H_

#include <map>
#include <string>
#include <vector>
#include <cstdint>

#include "define/sdltype.h"
#include "util/logger.h"

// resource manager
class ResManager {
 public:
  ResManager(const SDLRendererPtr &renderer) : renderer_(renderer) {}

  // load image from file
  void LoadImage(const std::string &name, const std::string &path);
  void LoadImage(const std::string &name,
                 std::vector<std::uint8_t> &bytes);
  // get image by name
  SDLTexturePtr &GetImage(const std::string &name) {
    auto it = images_.find(name);
    if (it == images_.end()) LOG_ERROR("invalid image name");
    return it->second;
  }

  // clear all images
  void Clear() { images_.clear(); }

 private:
  const SDLRendererPtr &renderer_;
  std::map<std::string, SDLTexturePtr> images_;
};

#endif  // ZODIA_CORE_RESMAN_H_
