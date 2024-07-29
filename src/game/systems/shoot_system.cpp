#include "shoot_system.h"

#include "game.h"
#include "game_utils.h"
#include "pools/entity_pool_base.h"
#include "core/input_manager.h"

#include "components/sprite_component.h"
#include "components/transform_component.h"
#include "components/shooter_component.h"
#include "components/projectile_component.h"
#include "components/rigid_body_component.h"
#include "components/enemy_component.h"
#include "components/animation_component.h"
#include "components/player_component.h"
#include "components/damage_dealer_component.h"
#include "components/collision_handler_component.h"

void SpawnProjectile(Entity entity, float shoot_offset, ShooterComponent& shooter_comp, TransformComponent& transform_comp)
{
  Entity projectile = Game::GetPool(EntityPoolType::PROJECTILE)->GetEntity();

  SpriteComponent& sprite_comp = projectile.GetComponent<SpriteComponent>();
  sprite_comp.current_frame = "";

  AnimationComponent& anim_comp = projectile.GetComponent<AnimationComponent>();
  anim_comp.params.animation_rate = 15.f;
  anim_comp.params.loop = true;
  anim_comp.stopped = false;
  anim_comp.started = true;

  if (entity.HasComponent<PlayerComponent>())
  {
    anim_comp.params.frames = { "laserBlue01", "laserBlue03", "laserBlue05", "laserBlue06", "laserBlue07" };
  }
  else
  {
    anim_comp.params.frames = { "laserRed01", "laserRed03", "laserRed05", "laserRed06", "laserRed07" };
  }

  TransformComponent& projectile_transform_comp = projectile.GetComponent<TransformComponent>();
  projectile_transform_comp.world_transform.SetRotation(transform_comp.world_transform.GetRotation());

  const glm::vec2& shoot_offset_vec = GameUtils::AngleToVec2(GameUtils::WrapAngle(transform_comp.world_transform.GetRotation() + shoot_offset));
  projectile_transform_comp.world_transform.position = transform_comp.world_transform.position + shoot_offset_vec * 70.f;

  ProjectileComponent& projectile_component = projectile.GetComponent<ProjectileComponent>();
  projectile_component.velocity = transform_comp.world_transform.GetForwardVector() * shooter_comp.projectile_speed;
  projectile_component.lifetime = shooter_comp.projectile_lifetime;
  projectile_component.spawn_time = GameUtils::GetTime();

  DamageDealerComponent& damage_dealer_component = projectile.GetComponent<DamageDealerComponent>();
  damage_dealer_component.damage = shooter_comp.projectile_damage;
  damage_dealer_component.owner = entity;

  shooter_comp.last_shoot_time = projectile_component.spawn_time;
}

void ShootSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<ShooterComponent, TransformComponent>();

  view.each([delta_time](Entity entity, ShooterComponent& shooter_comp, TransformComponent& transform_comp) {

    if (shooter_comp.shoot_triggered && GameUtils::GetTime() - shooter_comp.last_shoot_time > shooter_comp.shoot_delay)
    {
      if (shooter_comp.shots_count == 2)
      {
        SpawnProjectile(entity, -30.0f, shooter_comp, transform_comp);
        SpawnProjectile(entity, 30.0f, shooter_comp, transform_comp);
      }
      else if (shooter_comp.shots_count == 1)
      {
        SpawnProjectile(entity, 0.0f, shooter_comp, transform_comp);
      }
      else
      {
        SpawnProjectile(entity, 0.0f, shooter_comp, transform_comp);
        SpawnProjectile(entity, -30.0f, shooter_comp, transform_comp);
        SpawnProjectile(entity, 30.0f, shooter_comp, transform_comp);
      }

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
