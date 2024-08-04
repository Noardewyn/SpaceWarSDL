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
#include "components/input_component.h"
#include "components/movement_component.h"

Entity EnemiesPool::OnCreate()
{
  Entity entity = Game::CreateEntity("EnemyShip");

  Transform enemy_transform(POOL_OBJECTS_POSITION.x, POOL_OBJECTS_POSITION.y);

  entity.AddComponent<EnemyComponent>(EnemyType::ASTEROID_SMALL);
  entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);
  entity.AddComponent<TransformComponent>(enemy_transform);
  entity.AddComponent<CircleCollisionShapeComponent>(10.f);
  entity.AddComponent<CollisionHandlerComponent>(CollisionMask::ENEMY, CollisionMask(CollisionMask::PLAYER | CollisionMask::PROJECTILE));
  entity.AddComponent<DamageDealerComponent>(1.f, entity);
  entity.AddComponent<HealthComponent>(1.f);
  entity.AddComponent<PoolPropertyComponent>(EntityPoolType::ENEMY);
  entity.AddComponent<ThrusterComponent>(glm::vec2(-45.f, -45.f));
  entity.AddComponent<InputComponent>();
  entity.AddComponent<MovementComponent>();

  auto& rigidbody_comp = entity.AddComponent<RigidBodyComponent>(50.f);
  rigidbody_comp.ignore_list = { "Player", "EnemyAsteroid", "EnemyShip" };

  return entity;
}

void EnemiesPool::OnReset(Entity entity)
{
  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = POOL_OBJECTS_POSITION;

  auto& input_comp = entity.GetComponent<InputComponent>();
  input_comp.move_direction = glm::vec2(0.f, 0.f);
  input_comp.shoot_triggered = false;
  input_comp.steereng = 0.f;

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();
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

