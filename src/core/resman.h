#ifndef ZODIA_CORE_RESMAN_H_
#define ZODIA_CORE_RESMAN_H_

#include <map>
#include <string>

#include "define/sdltype.h"

class ResManager {
 public:
  ResManager(const SDLRendererPtr &renderer) : renderer_(renderer) {}

  // load image from file
  void LoadImage(const std::string &name, const std::string &path);
  // get image by name
  SDLTexturePtr &GetImage(const std::string &name) {
      return images_[name];
  }

 private:
  const SDLRendererPtr &renderer_;
  std::map<std::string, SDLTexturePtr> images_;
};

#endif  // ZODIA_CORE_RESMAN_H_
