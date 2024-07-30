#include "thruster_system.h"

#include "game.h"
#include "game_utils.h"
#include "core/entity.h"
#include "game/pools/entity_pool_base.h"
#include "gameplay_statics.h"

#include "components/sprite_component.h"
#include "components/transform_component.h"
#include "components/animation_component.h"
#include "components/inactive_component.h"
#include "components/thruster_component.h"
#include "components/death_component.h"

void ThrusterSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();

  // Render sprites
  auto view = reg.view<ThrusterComponent, TransformComponent>();

  view.each([](Entity entity, ThrusterComponent& thruster_comp, TransformComponent& transform_comp) {
    const glm::vec2 offset = transform_comp.world_transform.GetForwardVector() * thruster_comp.offset;
    const glm::vec2 thruster_pos = transform_comp.world_transform.position + offset;
    const float thruster_rot = GameUtils::WrapAngle(transform_comp.world_transform.GetRotation() + 180.f);
    const Transform thruster_transform(thruster_pos.x, thruster_pos.y, thruster_rot);

    Entity effect_entity = thruster_comp.effect_entity;

    if (Game::HasEntity(effect_entity))
    {
      if (entity.HasComponent<DeadComponent>())
      {
        thruster_comp.effect_entity = entt::null;
        Game::GetPool(EntityPoolType::EFFECTS)->FreeEntity(effect_entity);
        return;
      }

      auto& animation_comp = effect_entity.GetComponent<AnimationComponent>();
      if (thruster_comp.turn_on)
      {
        animation_comp.stopped = false;
      }
      else
      {
        animation_comp.stopped = true;
      }

      auto& thruster_transform_comp = effect_entity.GetComponent<TransformComponent>();
      thruster_transform_comp.world_transform = thruster_transform;
    }
    else
    {
      thruster_comp.effect_entity = GameplayStatics::SpawnEffect(thruster_transform, EffectType::THRUSTER);
    }
  });
}
