#pragma once
#include "core/asset_base.h"
#include "game_types.h"

#include <array>
#include <vector>

class GameConfigAsset : public AssetBase
{
public:
  GameConfigAsset(const std::string& path);
  virtual ~GameConfigAsset() = default;

  struct EnemiesPack
  {
    EnemyType enemy_type;
    int min_count;
    int max_count;
    std::vector<std::pair<CollectableType, float>> collectables;
  };

  struct WaveData
  {
    std::vector<EnemiesPack> packs;
  };

  std::vector<WaveData> waves;
  std::array<float, uint64_t(EnemyType::COUNT)> score_mapping;

  float collectable_score = 0.f;
  float delay_between_waves = 4.f;
};
