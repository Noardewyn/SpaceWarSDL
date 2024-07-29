#include "damage_system.h"
#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"
#include "core/entity.h"
#include "core/log.h"
#include "pools/entity_pool_base.h"

#include "components/transform_component.h"
#include "components/projectile_component.h"
#include "components/collision_handler_component.h"
#include "components/health_component.h"
#include "components/damage_dealer_component.h"
#include "components/player_component.h"
#include "components/death_component.h"
#include "components/pool_property_component.h"
#include "components/hud_single_component.h"
#include "components/inactive_component.h"

void DamageSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<TransformComponent, DamageDealerComponent, CollisionHandlerComponent>();

  view.each([](Entity entity, TransformComponent& transform_comp, DamageDealerComponent& damage_dealer_comp, CollisionHandlerComponent& collision_handler_comp) {
    if (entity.HasComponent<InactiveComponent>() || entity.HasComponent<DeadComponent>())
    {
      return;
    }

    for (Entity other_entity : collision_handler_comp.collided_entities)
    {
      if (!other_entity.HasComponent<HealthComponent>() ||
        other_entity.HasComponent<DeadComponent>() ||
        other_entity == damage_dealer_comp.owner ||
        other_entity.HasComponent<InactiveComponent>())
      {
        continue;
      }

      auto& health_comp = other_entity.GetComponent<HealthComponent>();
      health_comp.health = std::max(0.f, health_comp.health - damage_dealer_comp.damage);

      if (other_entity.HasComponent<PlayerComponent>())
      {
        if (Game::GetRootEntity().HasComponent<HUDSingleComponent>())
        {
          Game::GetRootEntity().GetComponent<HUDSingleComponent>().health = int(health_comp.health);
        }
      }

      if (entity.HasComponent<ProjectileComponent>())
      {
        bool is_player = damage_dealer_comp.owner.HasComponent<PlayerComponent>();
        GameplayStatics::SpawnEffect(transform_comp.world_transform, EffectType::PROJECTILE_HIT, is_player);
      }

      if (health_comp.health <= 0.f)
      {
        bool is_crash_damage = !entity.HasComponent<ProjectileComponent>();
        other_entity.AddComponent<DeadComponent>(is_crash_damage);
      }

      if (damage_dealer_comp.remove_after_damage)
      {
        if (entity.HasComponent<PoolPropertyComponent>())
        {
          EntityPoolType pool_type = entity.GetComponent<PoolPropertyComponent>().pool_type;
          Game::GetPool(pool_type)->FreeEntity(entity);
        }
        else
        {
          assert(false);
        }
      }
    }
  });
}
