#include "collectables_pool.h"
#include "game_utils.h"
#include "components/transform_component.h"
#include "components/sprite_component.h"
#include "components/circle_collision_shape_component.h"
#include "components/collision_handler_component.h"
#include "components/collectable_component.h"
#include "components/pool_property_component.h"

Entity CollectablesPool::OnCreate()
{
  Entity entity = Game::CreateEntity("Collectable");

  entity.AddComponent<TransformComponent>();
  entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);
  entity.AddComponent<CircleCollisionShapeComponent>(20.f);
  entity.AddComponent<CollisionHandlerComponent>(CollisionMask::COLLECTABLE, CollisionMask::PLAYER);
  entity.AddComponent<PoolPropertyComponent>(EntityPoolType::COLLECTABLE);

  auto& collectable_comp = entity.AddComponent<CollectableComponent>(CollectableType::NONE);
  collectable_comp.spawn_time = GameUtils::GetTime();

  return entity;
}

void CollectablesPool::OnReset(Entity entity)
{
  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = pool_objects_position;
}
