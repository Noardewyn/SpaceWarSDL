#pragma once
#include <string>
#include <memory>

template <typename T>
using Asset = std::shared_ptr<T>;

class AssetBase
{
public:
  AssetBase(const std::string& asset_path);
  virtual ~AssetBase();

  std::string GetPath() const;

  virtual void load()
  {
  };
  virtual void unload()
  {
  };

private:
  std::string asset_path;
};
