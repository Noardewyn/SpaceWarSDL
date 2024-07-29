#include "game/assets/sound_asset.h"
#include "core/log.h"

SoundAsset::SoundAsset(const std::string& path)
  : AssetBase(path)
{
  sound = Mix_LoadWAV(path.c_str());
  if (sound == nullptr)
  {
    LOG_ERROR("Failed to load sound asset! path = {}. SDL_mixer Error: {}", path, Mix_GetError());
  }
}

SoundAsset::~SoundAsset()
{
  Mix_FreeChunk(sound);
}

void SoundAsset::Play(int channel, int loops)
{
  Mix_PlayChannel(channel, sound, loops);
}
