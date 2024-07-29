#include "game/assets/game_config_asset.h"
#include "core/log.h"

#include <rapidyaml.hpp>
#include <fstream> 

EnemyType StringToEnemyType(const std::string& str)
{
  if (str == "ASTEROID_SMALL") return EnemyType::ASTEROID_SMALL;
  else if (str == "ASTEROID_MED") return EnemyType::ASTEROID_MED;
  else if (str == "ASTEROID_BIG") return EnemyType::ASTEROID_BIG;
  else if (str == "SHOOTER") return EnemyType::SHOOTER;
  else if (str == "KAMIKADZE") return EnemyType::KAMIKADZE;
  else if (str == "UFO") return EnemyType::UFO;

  assert(false);

  return EnemyType::ASTEROID_SMALL;
}

CollectableType StringToCollectableType(const std::string& str)
{
  if (str == "EXTRA_LIFE") return CollectableType::EXTRA_LIFE;
  else if (str == "MASS_DEATH") return CollectableType::MASS_DEATH;
  else if (str == "EXTRA_SHOT") return CollectableType::EXTRA_SHOT;
  else if (str == "RAPID_FIRE") return CollectableType::RAPID_FIRE;

  assert(false);

  return CollectableType::EXTRA_LIFE;
}

GameConfigAsset::GameConfigAsset(const std::string& path)
  : AssetBase(path)
{
  std::ifstream input_file(path);

  if (!input_file.is_open())
  {
    LOG_ERROR("GameConfigAsset: Can't open file {}", path.c_str());
  }

  const std::string content = std::move(std::string(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>()));
  input_file.close();

  const ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(content));

  tree["delay_between_waves"] >> delay_between_waves;
  tree["score"]["collectable"] >> collectable_score;

  for (ryml::ConstNodeRef wave : tree["waves"])
  {
    WaveData wave_data;

    for (const ryml::ConstNodeRef& pack : wave["packs"])
    {
      EnemiesPack enemy_pack;

      std::string enemy_type_str;
      pack["enemy_type"] >> enemy_type_str;
      enemy_pack.enemy_type = StringToEnemyType(enemy_type_str);

      pack["min_count"] >> enemy_pack.min_count;
      pack["max_count"] >> enemy_pack.max_count;

      if (pack.has_child("collectables"))
      {
        for (const ryml::ConstNodeRef& collectable : pack["collectables"])
        {
          std::string collectable_type_str;
          float spawn_chance = 0.f;
          collectable["type"] >> collectable_type_str;
          collectable["chance"] >> spawn_chance;

          enemy_pack.collectables.emplace_back(StringToCollectableType(collectable_type_str), spawn_chance);
        }
      }

      wave_data.packs.push_back(enemy_pack);
    }

    waves.push_back(wave_data);
  }

  for (const ryml::ConstNodeRef& score : tree["score"]["enemies"])
  {
    std::string key_str;
    score >> ryml::key(key_str);

    float score_value = 0.f;
    score >> score_value;

    EnemyType enemy_type = StringToEnemyType(key_str);
    score_mapping[int(enemy_type)] = score_value;
  }
}