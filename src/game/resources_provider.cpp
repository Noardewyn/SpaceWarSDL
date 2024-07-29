#include "resources_provider.h"
#include "core/asset_manager.h"

bool ResourcesProvider::LoadResources()
{
  // Assets loading
  background_texture = AssetManager::CreateAsset<TextureAsset>("images/2k_stars.jpg");
  menu_texture_asset = AssetManager::CreateAsset<TextureAsset>("images/bg_menu.png");
  gameplay_texture_asset = AssetManager::CreateAsset<TextureAsset>("images/atlases/base.png");
  gameplay_atlas_asset = AssetManager::CreateAsset<TextureAtlasAsset>("images/atlases/base.json");
  game_config_asset = AssetManager::CreateAsset<GameConfigAsset>("config/game_config.yaml");

  player_laser_sound = AssetManager::CreateAsset<SoundAsset>("sounds/sfx_laser1.ogg");
  enemy_laser_sound = AssetManager::CreateAsset<SoundAsset>("sounds/sfx_laser2.ogg");
  thruster_sound = AssetManager::CreateAsset<SoundAsset>("sounds/thrusterFire_003.ogg");
  player_hit_sound = AssetManager::CreateAsset<SoundAsset>("sounds/impact_player.ogg");
  enemy_hit_sound = AssetManager::CreateAsset<SoundAsset>("sounds/impact_enemy.ogg");
  explosion_sound = AssetManager::CreateAsset<SoundAsset>("sounds/explosionCrunch_000.ogg");
  powerup_appear_sound = AssetManager::CreateAsset<SoundAsset>("sounds/sfx_zap.ogg");
  powerup_collect_sound = AssetManager::CreateAsset<SoundAsset>("sounds/sfx_twoTone.ogg");

  game_music = AssetManager::CreateAsset<MusicAsset>("music/boss.ogg");
  menu_music = AssetManager::CreateAsset<MusicAsset>("music/finished_long.ogg");
  game_over_music = AssetManager::CreateAsset<MusicAsset>("music/death.ogg");

  return true;
}
