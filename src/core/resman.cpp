#include "core/resman.h"

#include <SDL2/SDL_image.h>

#include "util/logger.h"

void ResManager::LoadImage(const std::string &name,
                           const std::string &path) {
  auto image = SDLTexturePtr(
      IMG_LoadTexture(renderer_.get(), path.c_str()),
      [](SDL_Texture *texture) { SDL_DestroyTexture(texture); });
  if (!image) LOG_ERROR("failed to load image");
  images_[name] = image;
}
