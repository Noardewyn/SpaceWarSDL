#include "game_state_system.h"
#include "game.h"
#include "game_utils.h"
#include "core/entity.h"
#include "core/log.h"
#include "gameplay_statics.h"
#include "components/game_state_single_component.h"

void SpawnNextWave(GameStateSingleComponent& game_state_sc)
{
  game_state_sc.current_wave = std::min(game_state_sc.current_wave + 1, int(game_state_sc.game_config->waves.size() - 1));

  LOG_INFO("Spawn next wave: {}", game_state_sc.current_wave);

  const GameConfigAsset::WaveData& wave_data = game_state_sc.game_config->waves[game_state_sc.current_wave];

  for (const GameConfigAsset::EnemiesPack& enemies_pack : wave_data.packs)
  {
    int spawn_count = (int)GameUtils::Random((float)enemies_pack.min_count, (float)enemies_pack.max_count);

    for (int i = 0; i < spawn_count; i++)
    {
      CollectableType drop_collectable = CollectableType::NONE;
      for (const std::pair<CollectableType, float>& collectable : enemies_pack.collectables)
      {
        if (GameUtils::Random() < collectable.second)
        {
          drop_collectable = collectable.first;
          break;
        }
      }

      const glm::vec2 random_position = GameUtils::GetRandomSafePos(game_state_sc.player_pos, glm::vec2(Game::GetWindow().GetWidth(), Game::GetWindow().GetHeight()), 600.f, 2000.f);
      GameplayStatics::SpawnEnemy(enemies_pack.enemy_type, random_position, drop_collectable);
    }
  }
}

void GameStateSystem::Update(float delta_time)
{
  Entity root_entity = Game::GetRootEntity();

  auto& game_state_sc = root_entity.GetComponent<GameStateSingleComponent>();

  if (game_state_sc.player_death && GameUtils::GetTime() - game_state_sc.player_death_time > 3.f)
  {
    Game::ChangeScene(GameScenes::GAME_OVER);
    game_state_sc.time_scale = 0.15f;
  }
  else if (game_state_sc.enemies_left <= 0 && !game_state_sc.wave_spawning)
  {
    game_state_sc.enemies_left = 0;
    game_state_sc.wave_spawning = true;
    game_state_sc.wave_finished_time = GameUtils::GetTime();
  }

  if (game_state_sc.wave_spawning && GameUtils::GetTime() - game_state_sc.wave_finished_time > game_state_sc.game_config->delay_between_waves)
  {
    SpawnNextWave(game_state_sc);
    game_state_sc.wave_spawning = false;
  }
}
