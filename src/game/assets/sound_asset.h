#pragma once
#include "core/asset_base.h"

#include <SDL_mixer.h>

class SoundAsset : public AssetBase
{
public:
  SoundAsset(const std::string& path);
  ~SoundAsset() override;

  void Play(int channel = -1, int loops = 0);

private:
  Mix_Chunk* sound;
};
