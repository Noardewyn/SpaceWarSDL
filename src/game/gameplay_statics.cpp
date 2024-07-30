#include "gameplay_statics.h"
#include "game_utils.h"
#include "pools/entity_pool_base.h"
#include "core/entity.h"
#include "core/log.h"

#include "components/rigid_body_component.h"
#include "components/sprite_component.h"
#include "components/transform_component.h"
#include "components/enemy_component.h"
#include "components/player_component.h"
#include "components/health_component.h"
#include "components/game_state_single_component.h"
#include "components/circle_collision_shape_component.h"
#include "components/shooter_component.h"
#include "components/projectile_component.h"
#include "components/hud_single_component.h"
#include "components/animation_component.h"
#include "components/collectable_component.h"
#include "components/collision_handler_component.h"
#include "components/damage_dealer_component.h"
#include "components/pool_property_component.h"
#include "components/inactive_component.h"
#include "components/thruster_component.h"
#include "components/death_component.h"

#include <SDL_pixels.h>

void GameplayStatics::SpawnSun()
{
  Entity sun = Game::CreateEntity("Sun");
  auto& rigidbody_comp = sun.AddComponent<RigidBodyComponent>(900.f, true);
  rigidbody_comp.radial_velocity = 400.f;
  rigidbody_comp.radial_deceleration_coef = 1.0f;

  Transform sun_transform(Game::GetWindow().GetWidth() / 2.f, Game::GetWindow().GetHeight() / 2.f);
  sun_transform.scale = { 3.f, 3.f };

  sun.AddComponent<TransformComponent>(sun_transform);

  AnimationComponent::AnimationParams anim_params;
  anim_params.frames = { "star1", "star2", "star3" };
  anim_params.animation_rate = 10.f;
  anim_params.loop = true;

  sun.AddComponent<AnimationComponent>(anim_params);
  sun.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset);
}

void GameplayStatics::SpawnPlayer()
{
  Entity entity = Game::CreateEntity("Player");
  const std::string skin_name = "playerShip2_blue";
  auto& sprite_component = entity.AddComponent<SpriteComponent>(Game::GetResources().gameplay_texture_asset, Game::GetResources().gameplay_atlas_asset, skin_name);

  Transform player_transform(400, 200);
  player_transform.scale = { 0.75f, 0.75f };
  player_transform.SetRotation(135.f);

  entity.AddComponent<TransformComponent>(player_transform);
  entity.AddComponent<PlayerComponent>(glm::vec2(450.f, 450.f), 1000.f, 55.f);
  entity.AddComponent<ShooterComponent>(0.5f, 1.f, 1300.f, 1.2f);
  auto& rigidbody_comp = entity.AddComponent<RigidBodyComponent>(50.f);
  rigidbody_comp.ignore_list = { "Projectile", "EnemyShip", "EnemyAsteroid" };
  rigidbody_comp.max_radial_speed = 2000.f;
  rigidbody_comp.radial_deceleration_coef = 0.86f;

  float texture_width = player_transform.scale.x * sprite_component.texture_atlas->GetSpriteRect(skin_name).w / 2.3f;
  entity.AddComponent<CircleCollisionShapeComponent>(texture_width);
  entity.AddComponent<CollisionHandlerComponent>(CollisionMask::PLAYER, CollisionMask(CollisionMask::COLLECTABLE | CollisionMask::ENEMY | CollisionMask::PROJECTILE));
  entity.AddComponent<DamageDealerComponent>(10.f, entity);
  entity.AddComponent<ThrusterComponent>(glm::vec2(-50.f, -50.f));

  auto& health_comp = entity.AddComponent<HealthComponent>(5.f);

  if (Game::GetRootEntity().HasComponent<HUDSingleComponent>())
  {
    Game::GetRootEntity().GetComponent<HUDSingleComponent>().health = static_cast<int>(health_comp.health);
  }

  if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
  {
    Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_entity = entity;
  }
}

void SpawnEnemyShip(const glm::vec2& position, CollectableType collectable)
{
  Entity entity = Game::GetPool(EntityPoolType::ENEMY)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "EnemyShip";

  const float random_angle = GameUtils::Random(0.f, 360.f);
  const glm::vec2& random_direction = GameUtils::AngleToVec2(random_angle);

  auto& rigidbody_comp = entity.GetComponent<RigidBodyComponent>();
  rigidbody_comp.ignore_list = { "Player", "EnemyAsteroid", "EnemyShip" };
  rigidbody_comp.mass = 50.f;

  std::string frame_name = "playerShip1_orange";
  auto& sprite_component = entity.GetComponent<SpriteComponent>();
  sprite_component.current_frame = frame_name;

  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = position;
  transform_component.world_transform.scale = { 0.55f, 0.55f };
  transform_component.world_transform.SetRotation(random_angle);

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();
  enemy_comp.enemy_type = EnemyType::SHOOTER;
  enemy_comp.rotation_speed = 200.0;
  enemy_comp.acceleration = glm::vec2(400.f, 400.f);

  enemy_comp.ai_state = AIState::SEEK;
  enemy_comp.collectable_to_spawn = collectable;
  enemy_comp.action_length = 5.f;
  enemy_comp.action_delay = 0.2f;
  enemy_comp.last_action_time = GameUtils::GetTime();
  enemy_comp.level = 0;

  float texture_width = transform_component.world_transform.scale.x * sprite_component.texture_atlas->GetSpriteRect(frame_name).w / 1.7f;
  auto& collision_comp = entity.GetComponent<CircleCollisionShapeComponent>();
  collision_comp.radius = texture_width;

  auto& damage_dealer_comp = entity.GetComponent<DamageDealerComponent>();
  damage_dealer_comp.damage = 1.f;
  damage_dealer_comp.owner = entity;

  auto& health_comp = entity.GetComponent<HealthComponent>();
  health_comp.health = 2.f;

  auto& shooter_comp = entity.GetOrAddComponent<ShooterComponent>();
  shooter_comp.shoot_delay = 2.5f;
  shooter_comp.projectile_damage = 1.f;
  shooter_comp.projectile_speed = 700.f;
  shooter_comp.projectile_lifetime = 1.8f;

  auto& thruster_comp = entity.GetOrAddComponent<ThrusterComponent>();
  thruster_comp.offset = glm::vec2(-45.f, -45.f);
}

void SpawnEnemyKamikadzeShip(const glm::vec2& position, CollectableType collectable)
{
  Entity entity = Game::GetPool(EntityPoolType::ENEMY)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "EnemyShip";

  const float random_angle = GameUtils::Random(0.f, 360.f);
  const glm::vec2& random_direction = GameUtils::AngleToVec2(random_angle);

  auto& rigidbody_comp = entity.GetComponent<RigidBodyComponent>();
  rigidbody_comp.ignore_list = { "Player", "EnemyAsteroid", "EnemyShip" };
  rigidbody_comp.mass = 50.f;

  const std::string& frame_name = "playerShip3_red";
  auto& sprite_component = entity.GetComponent<SpriteComponent>();
  sprite_component.current_frame = frame_name;

  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = position;
  transform_component.world_transform.scale = { 0.55f, 0.55f };
  transform_component.world_transform.SetRotation(random_angle);

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();
  enemy_comp.enemy_type = EnemyType::KAMIKADZE;
  enemy_comp.rotation_speed = 800.0;
  enemy_comp.acceleration = glm::vec2(850.f, 850.f);
  enemy_comp.ai_state = AIState::SEEK;
  enemy_comp.action_length = 5.f;
  enemy_comp.action_delay = 0.2f;
  enemy_comp.last_action_time = GameUtils::GetTime();
  enemy_comp.collectable_to_spawn = collectable;
  enemy_comp.level = 0;

  float texture_width = transform_component.world_transform.scale.x * sprite_component.texture_atlas->GetSpriteRect(frame_name).w / 2.0f;
  auto& collision_comp = entity.GetComponent<CircleCollisionShapeComponent>();
  collision_comp.radius = texture_width;

  auto& damage_dealer_comp = entity.GetComponent<DamageDealerComponent>();
  damage_dealer_comp.damage = 1.f;
  damage_dealer_comp.owner = entity;

  auto& health_comp = entity.GetComponent<HealthComponent>();
  health_comp.health = 2.f;

  auto& thruster_comp = entity.GetOrAddComponent<ThrusterComponent>();
  thruster_comp.offset = glm::vec2(-45.f, -45.f);
}

void SpawnUfoShip(const glm::vec2& position, CollectableType collectable)
{
  Entity entity = Game::GetPool(EntityPoolType::ENEMY)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "EnemyShip";

  const float random_angle = GameUtils::Random(0.f, 360.f);
  const glm::vec2& random_direction = GameUtils::AngleToVec2(random_angle);

  auto& rigidbody_comp = entity.GetComponent<RigidBodyComponent>();
  rigidbody_comp.ignore_list = { "Player", "EnemyAsteroid", "EnemyShip" };
  rigidbody_comp.mass = 1000.f;

  const std::string& frame_name = "ufoRed";
  auto& sprite_component = entity.GetComponent<SpriteComponent>();
  sprite_component.current_frame = frame_name;

  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = position;
  transform_component.world_transform.scale = { 1.0f, 1.0f };
  transform_component.world_transform.SetRotation(random_angle);

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();
  enemy_comp.enemy_type = EnemyType::UFO;
  enemy_comp.rotation_speed = 300.0;
  enemy_comp.acceleration = glm::vec2(350.f, 350.f);
  enemy_comp.ai_state = AIState::MOVE;
  enemy_comp.action_length = 5.f;
  enemy_comp.action_delay = 0.2f;
  enemy_comp.last_action_time = GameUtils::GetTime();
  enemy_comp.collectable_to_spawn = collectable;
  enemy_comp.level = 0;

  float texture_width = transform_component.world_transform.scale.x * sprite_component.texture_atlas->GetSpriteRect(frame_name).w / 2.0f;
  auto& collision_comp = entity.GetComponent<CircleCollisionShapeComponent>();
  collision_comp.radius = texture_width;

  auto& damage_dealer_comp = entity.GetComponent<DamageDealerComponent>();
  damage_dealer_comp.damage = 1.f;
  damage_dealer_comp.owner = entity;

  auto& health_comp = entity.GetComponent<HealthComponent>();
  health_comp.health = 5.f;

  entity.RemoveComponent<ThrusterComponent>();
}

void SpawnAsteroid(const glm::vec2& position, int level, CollectableType collectable)
{
  Entity entity = Game::GetPool(EntityPoolType::ENEMY)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "EnemyAsteroid";

  const float random_angle = GameUtils::Random(0.f, 360.f);
  const glm::vec2& random_direction = GameUtils::AngleToVec2(random_angle);
  const float random_speed = GameUtils::Random(100.f, 400.f);
  const float random_rotation_speed = GameUtils::Random(-300.f, 300.f);

  std::string frame_name = rand() % 2 ? "meteorGrey" : "meteorBrown";
  EnemyType enemy_type = EnemyType::ASTEROID_SMALL;

  switch (level)
  {
  case 0:
    frame_name += "_small" + std::to_string(1 + rand() % 2);
    enemy_type = EnemyType::ASTEROID_SMALL;
    break;
  case 1:
    frame_name += "_med1";
    enemy_type = EnemyType::ASTEROID_MED;
    break;
  case 2:
    frame_name += "_big" + std::to_string(1 + rand() % 4);
    enemy_type = EnemyType::ASTEROID_BIG;
    break;
  default:
    assert(level < 3);
  }

  glm::vec2 random_velocity(random_direction * random_speed);

  auto& rigidbody_comp = entity.GetComponent<RigidBodyComponent>();
  rigidbody_comp.ignore_list = { "Player", "EnemyShip" };
  rigidbody_comp.velocity = random_velocity;
  rigidbody_comp.radial_velocity = random_rotation_speed;
  rigidbody_comp.radial_deceleration_coef = 1.0f;
  rigidbody_comp.mass = 100.f;

  auto& sprite_component = entity.GetComponent<SpriteComponent>();
  sprite_component.current_frame = frame_name;

  auto& transform_component = entity.GetComponent<TransformComponent>();
  transform_component.world_transform.position = position;
  transform_component.world_transform.scale = { 1.0f, 1.0f };
  transform_component.world_transform.SetRotation(random_angle);

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();
  enemy_comp.enemy_type = enemy_type;
  enemy_comp.rotation_speed = 300.0;
  enemy_comp.acceleration = glm::vec2(150.f, 150.f);
  enemy_comp.collectable_to_spawn = collectable;
  enemy_comp.level = level;

  float texture_width = transform_component.world_transform.scale.x * sprite_component.texture_atlas->GetSpriteRect(frame_name).w / 2.0f;
  auto& collision_comp = entity.GetComponent<CircleCollisionShapeComponent>();
  collision_comp.radius = texture_width;

  auto& damage_dealer_comp = entity.GetComponent<DamageDealerComponent>();
  damage_dealer_comp.damage = 1.f;
  damage_dealer_comp.owner = entity;

  auto& health_comp = entity.GetComponent<HealthComponent>();
  health_comp.health = 1.f;

  entity.RemoveComponent<ThrusterComponent>();
}

Entity SpawnExplosionEffect(Transform transform)
{
  Entity entity = Game::GetPool(EntityPoolType::EFFECTS)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "ExplosionEffect";

  auto& sprite_comp = entity.GetComponent<SpriteComponent>();
  sprite_comp.current_frame = "";

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  transform_comp.world_transform = transform;

  auto& animation_comp = entity.GetComponent<AnimationComponent>();
  animation_comp.params.animation_rate = 20.f;
  animation_comp.params.loop = false;
  animation_comp.params.auto_delete = true;
  animation_comp.params.frames = { "explosion00", "explosion01", "explosion02", "explosion03", "explosion04", "explosion05", "explosion06", "explosion07", "explosion08" };
  animation_comp.current_frame_id = 0;
  animation_comp.stopped = false;
  animation_comp.started = false;

  return entity;
}

Entity SpawnHitEffect(Transform transform, bool is_player)
{
  Entity entity = Game::GetPool(EntityPoolType::EFFECTS)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "ExplosionEffect";

  auto& sprite_comp = entity.GetComponent<SpriteComponent>();
  sprite_comp.current_frame = "";

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  transform_comp.world_transform = transform;
  transform_comp.world_transform.scale = glm::vec2(0.4f);

  auto& animation_comp = entity.GetComponent<AnimationComponent>();
  animation_comp.params.animation_rate = 10.f;
  animation_comp.params.loop = false;
  animation_comp.params.auto_delete = true;
  animation_comp.current_frame_id = 0;
  animation_comp.stopped = false;
  animation_comp.started = false;

  if (is_player)
  {
    animation_comp.params.frames = { "laserBlue08", "laserBlue09", "laserBlue10", "laserBlue11" };
  }
  else
  {
    animation_comp.params.frames = { "laserRed08", "laserRed09", "laserRed10", "laserRed11" };
  }

  return entity;
}

Entity SpawnThrusterEffect(Transform transform)
{
  Entity entity = Game::GetPool(EntityPoolType::EFFECTS)->GetEntity();
  entity.GetComponent<TagComponent>().tag = "ThrusterEffect";

  auto& sprite_comp = entity.GetComponent<SpriteComponent>();
  sprite_comp.current_frame = "";

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  transform_comp.world_transform = transform;

  auto& animation_comp = entity.GetComponent<AnimationComponent>();
  animation_comp.params.animation_rate = 20.f;
  animation_comp.params.loop = true;
  animation_comp.params.auto_delete = false;
  animation_comp.params.frames = { "fire01", "fire02", "fire03", "fire04", "fire05", "fire06", "fire07" };
  animation_comp.current_frame_id = 0;
  animation_comp.stopped = false;
  animation_comp.started = false;

  return entity;
}

void GameplayStatics::SpawnCollectable(const Transform& transform, CollectableType collectable_type)
{
  std::string skin_name;

  switch (collectable_type)
  {
  case CollectableType::NONE:
    return;
    break;
  case CollectableType::EXTRA_LIFE:
    skin_name = "pill_blue";
    break;
  case CollectableType::MASS_DEATH:
    skin_name = "powerupRed_shield";
    break;
  case CollectableType::EXTRA_SHOT:
    skin_name = "star_gold";
    break;
  case CollectableType::RAPID_FIRE:
    skin_name = "bolt_gold";
    break;
  default:
    assert(false);
    break;
  }

  Entity entity = Game::GetPool(EntityPoolType::COLLECTABLE)->GetEntity();

  auto& sprite_comp = entity.GetComponent<SpriteComponent>();
  sprite_comp.current_frame = skin_name;
  sprite_comp.color = { 255, 255, 255, 255 };

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  transform_comp.world_transform = transform;
  transform_comp.world_transform.scale = glm::vec2(1.0f);

  auto& collectable_comp = entity.GetComponent<CollectableComponent>();
  collectable_comp.spawn_time = GameUtils::GetTime();
  collectable_comp.collectable_type = collectable_type;

  Game::GetResources().powerup_appear_sound->Play();
}

void GameplayStatics::SpawnEnemy(EnemyType enemy_type, const glm::vec2& position, CollectableType collectable)
{
  LOG_INFO("Spawn Enemy");

  switch (enemy_type)
  {
  case EnemyType::ASTEROID_SMALL:
    SpawnAsteroid(position, 0, collectable);
    break;
  case EnemyType::ASTEROID_MED:
    SpawnAsteroid(position, 1, collectable);
    break;
  case EnemyType::ASTEROID_BIG:
    SpawnAsteroid(position, 2, collectable);
    break;
  case EnemyType::SHOOTER:
    SpawnEnemyShip(position, collectable);
    break;
  case EnemyType::KAMIKADZE:
    SpawnEnemyKamikadzeShip(position, collectable);
    break;
  case EnemyType::UFO:
    SpawnUfoShip(position, collectable);
    break;
  case EnemyType::COUNT:
    break;
  default:
    break;
  }

  auto& game_state_sc = Game::GetRootEntity().GetComponent<GameStateSingleComponent>();
  game_state_sc.enemies_left++;
}

void GameplayStatics::ApplyCollectable(CollectableType collectable_type)
{
  Entity player_entity;
  if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
  {
    player_entity = Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_entity;
  }
  else
  {
    return;
  }

  if (collectable_type != CollectableType::NONE)
  {
    Game::GetResources().powerup_collect_sound->Play();
  }

  switch (collectable_type)
  {
  case CollectableType::EXTRA_LIFE:
  {
    LOG_INFO("Apply collectable: EXTRA_LIFE");

    auto& health_comp = player_entity.GetComponent<HealthComponent>();
    health_comp.health++;

    if (Game::GetRootEntity().HasComponent<HUDSingleComponent>())
    {
      Game::GetRootEntity().GetComponent<HUDSingleComponent>().health = static_cast<int>(health_comp.health);
    }

    break;
  }
  case CollectableType::MASS_DEATH:
  {
    LOG_INFO("Apply collectable: MASS_DEATH");

    KillAllEnemies();
    break;
  }
  case CollectableType::EXTRA_SHOT:
  {
    LOG_INFO("Apply collectable: EXTRA_SHOT");

    auto& shooter_comp = player_entity.GetComponent<ShooterComponent>();
    shooter_comp.shots_count = std::min(shooter_comp.shots_count + 1, 3);
    break;
  }
  case CollectableType::RAPID_FIRE:
  {
    LOG_INFO("Apply collectable: RAPID_FIRE");

    auto& shooter_comp = player_entity.GetComponent<ShooterComponent>();
    const float shoot_delay_cap = 0.1f;
    shooter_comp.shoot_delay = glm::max(shoot_delay_cap, shooter_comp.shoot_delay * 0.8f);
    break;
  }
  default:
    assert(false);
    break;
  }
}

void GameplayStatics::KillAllEnemies()
{
  entt::registry& reg = Game::GetRegistry();

  if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
  {
    Game::GetRootEntity().GetComponent<GameStateSingleComponent>().enemies_left = 0;
  }

  for (Entity entity : reg.view<TransformComponent, EnemyComponent>())
  {
    if (!entity.HasComponent<InactiveComponent>())
    {
      entity.AddComponent<DeadComponent>(false);
    }
  }
}

void GameplayStatics::OnEnemyDead(Entity entity, bool is_crash_damage)
{
  LOG_INFO("Enemy Dead!");

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  GameplayStatics::SpawnEffect(transform_comp.world_transform, EffectType::EXPLOSION);

  auto& enemy_comp = entity.GetComponent<EnemyComponent>();

  if (Game::GetRootEntity().HasComponent<HUDSingleComponent>())
  {
    float score = Game::GetResources().game_config_asset->score_mapping[size_t(enemy_comp.enemy_type)];
    Game::GetRootEntity().GetComponent<HUDSingleComponent>().score += int(score);
  }

  Game::GetRootEntity().GetComponent<GameStateSingleComponent>().enemies_left--;

  if (!is_crash_damage)
  {
    if (enemy_comp.level == 1)
    {
      GameplayStatics::SpawnEnemy(EnemyType::ASTEROID_SMALL, transform_comp.world_transform.position);
      GameplayStatics::SpawnEnemy(EnemyType::ASTEROID_SMALL, transform_comp.world_transform.position);
    }
    else if (enemy_comp.level == 2)
    {
      GameplayStatics::SpawnEnemy(EnemyType::ASTEROID_MED, transform_comp.world_transform.position);
      GameplayStatics::SpawnEnemy(EnemyType::ASTEROID_MED, transform_comp.world_transform.position);
    }

    GameplayStatics::SpawnCollectable(Transform(transform_comp.world_transform.position.x, transform_comp.world_transform.position.y), enemy_comp.collectable_to_spawn);
  }

  if (entity.HasComponent<PoolPropertyComponent>())
  {
    EntityPoolType pool_type = entity.GetComponent<PoolPropertyComponent>().pool_type;
    Game::GetPool(pool_type)->FreeEntity(entity);
  }
}

void GameplayStatics::OnPlayerDead(Entity entity)
{
  LOG_INFO("Player Dead!");

  auto& transform_comp = entity.GetComponent<TransformComponent>();
  GameplayStatics::SpawnEffect(transform_comp.world_transform, EffectType::EXPLOSION);

  if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
  {
    auto& game_state_sc = Game::GetRootEntity().GetComponent<GameStateSingleComponent>();

    game_state_sc.player_death = true;
    game_state_sc.player_death_time = GameUtils::GetTime();
  }

  if (Mix_Playing(THRUSTER_SOUND_CHANNEL))
  {
    Mix_HaltChannel(THRUSTER_SOUND_CHANNEL);
  }

  Game::DestroyEntity(entity);
}

Entity GameplayStatics::SpawnEffect(const Transform& transform, EffectType effect_type, bool is_player)
{
  switch (effect_type)
  {
  case EffectType::EXPLOSION:
  {
    Game::GetResources().explosion_sound->Play();

    return SpawnExplosionEffect(transform);
  }
  case EffectType::PROJECTILE_HIT:
  {
    if (is_player)
    {
      Game::GetResources().player_hit_sound->Play();
    }
    else
    {
      Game::GetResources().enemy_hit_sound->Play();
    }

    return SpawnHitEffect(transform, is_player);
  }
  case EffectType::THRUSTER:
  {
    return SpawnThrusterEffect(transform);
  }
  default:
    assert(false);
    break;
  }

  return Entity();
}
