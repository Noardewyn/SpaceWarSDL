#include "game/assets/texture_asset.h"
#include "Game.h"

TextureAsset::TextureAsset(const std::string& path)
  : AssetBase(path),
  texture(Game::GetRenderer(), path)
{

}