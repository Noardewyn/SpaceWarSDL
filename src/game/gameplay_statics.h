#pragma once
#include "core/transform.h"
#include "core/entity.h"

#include <glm.hpp>

const int THRUSTER_SOUND_CHANNEL = 14;

class GameplayStatics
{
public:
  struct ProjectileSpawnParams
  {
    Entity owner;
    Transform transform;
    float damage = 0.f;
    float speed = 0.f;
    float lifetime = 0.f;
  };

  static void SpawnSun();
  static void SpawnPlayer();
  static void SpawnEnemy(EnemyType enemy_type, const glm::vec2& postion, CollectableType collectable = CollectableType::NONE);
  static void SpawnProjectile(ProjectileSpawnParams& params, float shoot_offset);
  static Entity SpawnEffect(const Transform& transform, EffectType effect_type, bool is_player = true);
  static void SpawnCollectable(const Transform& transform, CollectableType collectable_type);

  static void ApplyCollectable(CollectableType collectable_type);

  static void KillAllEnemies();

  static void OnEnemyDead(Entity entity, bool is_crash_damage);
  static void OnPlayerDead(Entity entity);
};
