#include "core/asset_base.h"
#include "core/log.h"

AssetBase::AssetBase(const std::string& asset_path)
  : asset_path(asset_path)
{

}

AssetBase::~AssetBase()
{
  LOG_INFO("Asset closed: {}", asset_path);
}

std::string AssetBase::GetPath() const
{
  return asset_path;
}
