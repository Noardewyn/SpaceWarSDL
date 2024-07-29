#include "effects_pool.h"
#include "components/transform_component.h"
#include "components/sprite_component.h"
#include "components/animation_component.h"
#include "components/pool_property_component.h"
#include "core/entity.h"

Entity EffectsPool::OnCreate()
{
  Entity entity = Game::CreateEntity("ExplosionEffect");
  entity.AddComponent<TransformComponent>();
  entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);

  AnimationComponent::AnimationParams anim_params;
  anim_params.animation_rate = 20.f;
  anim_params.loop = false;
  anim_params.auto_delete = true;
  anim_params.frames = { "explosion00", "explosion01", "explosion02", "explosion03", "explosion04", "explosion05", "explosion06", "explosion07", "explosion08" };

  entity.AddComponent<AnimationComponent>(anim_params);
  entity.AddComponent<PoolPropertyComponent>(EntityPoolType::EFFECTS);

  return entity;
}

void EffectsPool::OnReset(Entity entity)
{
  TransformComponent& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = pool_objects_position;
}
