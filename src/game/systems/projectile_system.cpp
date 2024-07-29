#include "projectile_system.h"
#include "game_utils.h"
#include "pools/entity_pool_base.h"
#include "core/entity.h"

#include "components/transform_component.h"
#include "components/projectile_component.h"
#include "components/inactive_component.h"

void ProjectileSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<ProjectileComponent, TransformComponent>();

  view.each([delta_time](Entity entity, ProjectileComponent& projectile_comp, TransformComponent& transform_comp) {

    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    if (projectile_comp.lifetime > 0.f && GameUtils::GetTime() - projectile_comp.spawn_time > projectile_comp.lifetime)
    {
      Game::GetPool(EntityPoolType::PROJECTILE)->FreeEntity(entity);
      return;
    }

    transform_comp.world_transform.position += projectile_comp.velocity * delta_time;
    transform_comp.world_transform.position = GameUtils::WrapCoordinates(transform_comp.world_transform.position, glm::vec2(Game::GetWindow().GetWidth(), Game::GetWindow().GetHeight()));
  });
}
