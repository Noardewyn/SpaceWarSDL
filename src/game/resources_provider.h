#pragma once
#include "assets/texture_atlas_asset.h"
#include "assets/texture_asset.h"
#include "assets/game_config_asset.h"
#include "assets/sound_asset.h"
#include "assets/music_asset.h"

class ResourcesProvider
{
public:
  // Assets
  Asset<TextureAtlasAsset> gameplay_atlas_asset;
  Asset<TextureAsset> gameplay_texture_asset;
  Asset<TextureAsset> background_texture;
  Asset<TextureAsset> menu_texture_asset;

  Asset<GameConfigAsset> game_config_asset;

  Asset<SoundAsset> player_laser_sound;
  Asset<SoundAsset> enemy_laser_sound;
  Asset<SoundAsset> thruster_sound;
  Asset<SoundAsset> player_hit_sound;
  Asset<SoundAsset> enemy_hit_sound;
  Asset<SoundAsset> explosion_sound;
  Asset<SoundAsset> powerup_appear_sound;
  Asset<SoundAsset> powerup_collect_sound;

  Asset<MusicAsset> menu_music;
  Asset<MusicAsset> game_music;
  Asset<MusicAsset> game_over_music;

  bool LoadResources();
};
