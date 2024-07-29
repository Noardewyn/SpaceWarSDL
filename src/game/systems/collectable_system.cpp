#include "collectable_system.h"
#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"
#include "core/entity.h"
#include "pools/entity_pool_base.h"

#include "components/collectable_component.h"
#include "components/transform_component.h"
#include "components/sprite_component.h"
#include "components/collision_handler_component.h"
#include "components/player_component.h"
#include "components/pool_property_component.h"
#include "components/inactive_component.h"

void CollectableSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto collisions_view = reg.view<CollectableComponent, CollisionHandlerComponent>();

  // Handle collisions with powerups
  collisions_view.each([delta_time](Entity entity, CollectableComponent& collectable_comp, CollisionHandlerComponent& collision_comp) {
    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    for (Entity collided_entity : collision_comp.collided_entities)
    {
      if (collided_entity.HasComponent<PlayerComponent>())
      {
        GameplayStatics::ApplyCollectable(collectable_comp.collectable_type);

        if (entity.HasComponent<PoolPropertyComponent>())
        {
          EntityPoolType pool_type = entity.GetComponent<PoolPropertyComponent>().pool_type;
          Game::GetPool(pool_type)->FreeEntity(entity);
        }

        return;
      }
      else
      {
        assert(false);
      }
    }
  });

  auto visual_view = reg.view<CollectableComponent, TransformComponent, SpriteComponent>();

  // Update powerups visuals and lifetime
  visual_view.each([delta_time](Entity entity, CollectableComponent& collectable_comp, TransformComponent& transform_comp, SpriteComponent& sprite_comp) {
    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    const float time_sinse_spawn = GameUtils::GetTime() - collectable_comp.spawn_time;
    float scale = GameUtils::Remap(glm::cos(GameUtils::GetTime() * 10.f), -1.0f, 1.0f, 0.7f, 1.2f);
    transform_comp.world_transform.scale = glm::vec2(scale, scale);

    if (collectable_comp.lifetime - time_sinse_spawn < 3.f)
    {
      scale = GameUtils::Remap(glm::cos(GameUtils::GetTime() * 20.f), -1.0f, 1.0f, 0.3f, 0.8f);
      sprite_comp.color.r = int(scale * 255);
      sprite_comp.color.g = int(scale * 255);
      sprite_comp.color.b = int(scale * 255);
      sprite_comp.color.a = int(scale * 255);
    }

    if (time_sinse_spawn > collectable_comp.lifetime)
    {
      if (entity.HasComponent<PoolPropertyComponent>())
      {
        EntityPoolType pool_type = entity.GetComponent<PoolPropertyComponent>().pool_type;
        Game::GetPool(pool_type)->FreeEntity(entity);
      }
    }
  });
}
