#include "core/resman.h"

#include <stdexcept>
#include <SDL2/SDL_image.h>

#include "util/logger.h"

void ResManager::LoadImage(const std::string &name,
                           const std::string &path) {
  auto image = SDLTexturePtr(
      IMG_LoadTexture(renderer_.get(), path.c_str()),
      [](SDL_Texture *texture) { SDL_DestroyTexture(texture); });
  if (!image) {
    Logger::LogError("ResManager::LoadImage");
    throw std::runtime_error("failed to load image");
  }
  images_[name] = image;
}
