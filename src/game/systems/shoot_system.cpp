#include "shoot_system.h"

#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"

#include "components/transform_component.h"
#include "components/shooter_component.h"
#include "components/input_component.h"
#include "components/enemy_component.h"

void ShootSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<ShooterComponent, InputComponent, TransformComponent>();

  view.each([delta_time](Entity entity, ShooterComponent& shooter_comp, InputComponent& input_comp, TransformComponent& transform_comp) {

    if (input_comp.shoot_triggered && GameUtils::GetTime() - shooter_comp.last_shoot_time > shooter_comp.shoot_delay)
    {
      GameplayStatics::ProjectileSpawnParams params;
      params.damage = shooter_comp.projectile_damage;
      params.lifetime = shooter_comp.projectile_lifetime;
      params.speed = shooter_comp.projectile_speed;
      params.transform = transform_comp.world_transform;
      params.owner = entity;

      if (shooter_comp.shots_count == 2)
      {

        GameplayStatics::SpawnProjectile(params, -30.0f);
        GameplayStatics::SpawnProjectile(params, 30.0f);
      }
      else if (shooter_comp.shots_count == 1)
      {
        GameplayStatics::SpawnProjectile(params, 0.0f);
      }
      else
      {
        GameplayStatics::SpawnProjectile(params, 0.0f);
        GameplayStatics::SpawnProjectile(params, -30.0f);
        GameplayStatics::SpawnProjectile(params, 30.0f);
      }

      shooter_comp.last_shoot_time = GameUtils::GetTime();

      if (entity.HasComponent<EnemyComponent>())
      {
        Game::GetResources().enemy_laser_sound->Play();
      }
      else
      {
        Game::GetResources().player_laser_sound->Play();
      }
    }
  });
}
