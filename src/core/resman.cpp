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

void ResManager::LoadImage(const std::string &name,
                           std::vector<std::uint8_t> &bytes) {
  SDL_RWops *rw = SDL_RWFromMem(bytes.data(), bytes.size());
  auto image = SDLTexturePtr(
      IMG_LoadTexture_RW(renderer_.get(), rw, 1),
      [](SDL_Texture *texture) { SDL_DestroyTexture(texture); });
  if (!image) LOG_ERROR("failed to load image");
  images_[name] = image;
}
