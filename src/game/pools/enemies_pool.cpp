#include "enemies_pool.h"
#include "game.h"
#include "game_utils.h"
#include "components/transform_component.h"
#include "components/rigid_body_component.h"
#include "components/sprite_component.h"
#include "components/enemy_component.h"
#include "components/circle_collision_shape_component.h"
#include "components/collision_handler_component.h"
#include "components/damage_dealer_component.h"
#include "components/health_component.h"
#include "components/shooter_component.h"
#include "components/projectile_component.h"
#include "components/pool_property_component.h"
#include "components/death_component.h"
#include "components/thruster_component.h"

Entity EnemiesPool::OnCreate()
{
  Entity entity = Game::CreateEntity("EnemyShip");

  RigidBodyComponent& rigidbody_comp = entity.AddComponent<RigidBodyComponent>(50.f);
  rigidbody_comp.ignore_list = { "Player", "EnemyAsteroid", "EnemyShip" };

  SpriteComponent& sprite_component = entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);

  Transform enemy_transform(pool_objects_position.x, pool_objects_position.y);

  entity.AddComponent<TransformComponent>(enemy_transform);

  EnemyComponent& enemy_comp = entity.AddComponent<EnemyComponent>(EnemyType::ASTEROID_SMALL);

  entity.AddComponent<CircleCollisionShapeComponent>(10.f);
  entity.AddComponent<CollisionHandlerComponent>(CollisionMask::ENEMY, CollisionMask(CollisionMask::PLAYER | CollisionMask::PROJECTILE));
  entity.AddComponent<DamageDealerComponent>(1.f, entity);
  entity.AddComponent<HealthComponent>(1.f);
  entity.AddComponent<PoolPropertyComponent>(EntityPoolType::ENEMY);
  entity.AddComponent<ThrusterComponent>(glm::vec2(-45.f, -45.f));

  return entity;
}

void EnemiesPool::OnReset(Entity entity)
{
  TransformComponent& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = pool_objects_position;

  EnemyComponent& enemy_comp = entity.GetComponent<EnemyComponent>();
  enemy_comp.last_state_change_time = 0.f;

  if (entity.HasComponent<ShooterComponent>())
  {
    entity.RemoveComponent<ShooterComponent>();
  }
  if (entity.HasComponent<DeadComponent>())
  {
    entity.RemoveComponent<DeadComponent>();
  }
  if (entity.HasComponent<ThrusterComponent>())
  {
    entity.RemoveComponent<ThrusterComponent>();
  }
}

