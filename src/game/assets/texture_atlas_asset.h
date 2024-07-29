#pragma once
#include "core/asset_base.h"

#include <SDL_rect.h>
#include <unordered_map>

class TextureAtlasAsset : public AssetBase
{
public:
  TextureAtlasAsset(const std::string& path);
  virtual ~TextureAtlasAsset() = default;

  SDL_Rect GetSpriteRect(const std::string& name);

private:
  std::unordered_map<std::string, SDL_Rect> frames;
};
