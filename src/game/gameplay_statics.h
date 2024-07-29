#pragma once
#include "core/transform.h"
#include "core/entity.h"

#include <glm.hpp>

class GameplayStatics
{
public:
  static void SpawnSun();
  static void SpawnPlayer();
  static void SpawnEnemy(EnemyType enemy_type, const glm::vec2& postion, CollectableType collectable = CollectableType::NONE);
  static void SpawnEffect(const Transform& transform, EffectType effect_type, bool is_player = true);
  static void SpawnCollectable(const Transform& transform, CollectableType collectable_type);

  static void ApplyCollectable(CollectableType collectable_type);

  static void KillAllEnemies();

  static void OnEnemyDead(Entity entity, bool is_crash_damage);
  static void OnPlayerDead(Entity entity);
};
