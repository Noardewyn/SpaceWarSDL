#pragma once

#include "game/assets/texture_asset.h"
#include "game/assets/texture_atlas_asset.h"

struct SpriteComponent
{
  Asset<TextureAsset> texture;
  Asset<TextureAtlasAsset> texture_atlas;
  std::string current_frame;

  SDL_Color color = { 255, 255, 255, 255 };

  SpriteComponent(Asset<TextureAsset> texture, Asset<TextureAtlasAsset> texture_atlas, const std::string& frame_name = "")
    : texture(texture),
    texture_atlas(texture_atlas),
    current_frame(frame_name)
  {
  }
};
