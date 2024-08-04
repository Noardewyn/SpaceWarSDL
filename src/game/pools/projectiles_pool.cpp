#include "projectiles_pool.h"
#include "game.h"

#include "components/sprite_component.h"
#include "components/transform_component.h"
#include "components/projectile_component.h"
#include "components/animation_component.h"
#include "components/damage_dealer_component.h"
#include "components/circle_collision_shape_component.h"
#include "components/collision_handler_component.h"
#include "components/pool_property_component.h"

Entity ProjectilesPool::OnCreate()
{
  Entity entity = Game::CreateEntity("Projectile");

  SpriteComponent& sprite_component = entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);
  entity.AddComponent<TransformComponent>(Transform(POOL_OBJECTS_POSITION.x, POOL_OBJECTS_POSITION.y));
  entity.AddComponent<ProjectileComponent>(glm::vec2(0.0f, 0.0f), 1.f);
  entity.AddComponent<AnimationComponent>();
  entity.AddComponent<DamageDealerComponent>(1.f, entity, true);
  entity.AddComponent<CircleCollisionShapeComponent>(10.f);
  entity.AddComponent<CollisionHandlerComponent>(CollisionMask::PROJECTILE, CollisionMask(CollisionMask::PLAYER | CollisionMask::ENEMY));
  entity.AddComponent<PoolPropertyComponent>(EntityPoolType::PROJECTILE);

  return entity;
}

void ProjectilesPool::OnReset(Entity entity)
{
  TransformComponent& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = POOL_OBJECTS_POSITION;

  ProjectileComponent& projectile_component = entity.GetComponent<ProjectileComponent>();
  projectile_component.velocity = glm::vec2(0.f, 0.f);
  projectile_component.lifetime = 0.f;
}

