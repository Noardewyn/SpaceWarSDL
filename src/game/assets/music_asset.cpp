#include "game/assets/music_asset.h"
#include "core/log.h"

MusicAsset::MusicAsset(const std::string& path)
  : AssetBase(path)
{
  music = Mix_LoadMUS(path.c_str());
  if (music == nullptr)
  {
    LOG_ERROR("Failed to load music asset! path = {}. SDL_mixer Error: {}", path, Mix_GetError());
  }
}

MusicAsset::~MusicAsset()
{
  Mix_FreeMusic(music);
}

void MusicAsset::Play(int loops)
{
  Mix_PlayMusic(music, loops);
}
