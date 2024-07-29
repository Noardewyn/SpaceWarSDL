#include "core/asset_manager.h"

std::string AssetManager::assets_path = "";
std::unordered_map<std::string, std::weak_ptr<AssetBase>> AssetManager::assets;
std::list<std::shared_ptr<AssetBase>> AssetManager::loaded_assets;

std::string AssetManager::AssetsPath()
{
  return assets_path;
}

void AssetManager::SetAssetsPath(const std::string& path)
{
  assets_path = path;
}

std::string AssetManager::TruncateBasePath(const std::string& path)
{
  std::string truncated_path = path;
  truncated_path.erase(truncated_path.find(AssetManager::AssetsPath()), AssetManager::AssetsPath().size());

  return truncated_path;
}

void AssetManager::Clear()
{
  loaded_assets.clear();
  assets.clear();
}
