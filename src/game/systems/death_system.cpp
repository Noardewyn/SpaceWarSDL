#include "death_system.h"
#include "game.h"
#include "gameplay_statics.h"

#include "components/death_component.h"
#include "components/player_component.h"
#include "components/enemy_component.h"
#include "components/inactive_component.h"

void DeathSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<DeadComponent>();

  view.each([](Entity entity, DeadComponent& dead_comp) {
    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    if (entity.HasComponent<EnemyComponent>())
    {
      GameplayStatics::OnEnemyDead(entity, dead_comp.is_crash_damage);
    }
    else if (entity.HasComponent<PlayerComponent>())
    {
      GameplayStatics::OnPlayerDead(entity);
    }
  });
}
