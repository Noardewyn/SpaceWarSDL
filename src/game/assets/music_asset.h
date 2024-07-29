#pragma once
#include "core/asset_base.h"

#include <SDL_mixer.h>

class MusicAsset : public AssetBase
{
public:
  MusicAsset(const std::string& path);
  ~MusicAsset() override;

  void Play(int loops = -1);

private:
  Mix_Music* music = nullptr;
};
