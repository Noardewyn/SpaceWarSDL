#pragma once
#include "core/asset_base.h"
#include "core/texture.h"

class TextureAsset : public AssetBase
{
public:
  TextureAsset(const std::string& path);
  virtual ~TextureAsset() = default;

  Texture texture;
};
