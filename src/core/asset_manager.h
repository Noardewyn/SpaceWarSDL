#pragma once
#include "core/asset_base.h"
#include "core/log.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <list>

class AssetManager
{
public:
  static std::string AssetsPath();
  static void SetAssetsPath(const std::string& path);

  template<typename T, typename... Args>
  static Asset<T> CreateAsset(const std::string& path, Args&&... params)
  {
    auto asset = assets.find(path);

    if (asset != assets.end())
    {
      return std::dynamic_pointer_cast<T>(asset->second.lock());
    }
    else
    {
      loaded_assets.emplace_back(new T(AssetsPath() + path, std::forward<Args>(params)...));
      assets.insert({ path, loaded_assets.back() });
      loaded_assets.back()->load();
      LOG_INFO("[AssetsManager] New asset loaded: {}", path);

      return std::dynamic_pointer_cast<T>(loaded_assets.back());
    }
  }

  template<typename T>
  static Asset<T> GetAsset(const std::string& path)
  {
    auto asset = assets.find(path);

    if (asset != assets.end())
    {
      return std::dynamic_pointer_cast<T>(asset->second.lock());
    }
    else
    {
      return {};
    }
  }

  static std::string TruncateBasePath(const std::string& path);

  static void Clear();

private:
  AssetManager() = default;

  static std::string assets_path;
  static std::unordered_map<std::string, std::weak_ptr<AssetBase>> assets;
  static std::list<std::shared_ptr<AssetBase>> loaded_assets;
};
