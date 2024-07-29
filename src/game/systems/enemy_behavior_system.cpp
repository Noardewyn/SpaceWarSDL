#include "systems/enemy_behavior_system.h"

#include "game.h"
#include "game_utils.h"
#include "core/log.h"
#include "core/entity.h"

#include "components/transform_component.h"
#include "components/enemy_component.h"
#include "components/rigid_body_component.h"
#include "components/game_state_single_component.h"
#include "components/shooter_component.h"

float TargetAngleDiff(const glm::vec2& target_postion, const glm::vec2& our_position, float our_rotation)
{
  const glm::vec2 dir_to_target = target_postion - our_position;
  const float target_rotation = GameUtils::Vec2ToAngle(normalize(glm::vec2(dir_to_target.x, -dir_to_target.y)));

  return GameUtils::AnglesDiff(target_rotation, our_rotation);
}

bool UpdateActionPossible(EnemyComponent& enemy_comp)
{
  const float time_since_last_action = GameUtils::GetTime() - enemy_comp.last_action_time;

  if (time_since_last_action > enemy_comp.action_delay)
  {
    if (time_since_last_action < enemy_comp.action_delay + enemy_comp.action_length)
    {
      return true;
    }
    else
    {
      enemy_comp.last_action_time = GameUtils::GetTime();
      enemy_comp.action_delay = GameUtils::Random(1.f, 3.f);
      enemy_comp.action_length = GameUtils::Random(2.f, 4.f);
    }
  }

  return false;
}

void RotateToTarget(float target_rotation, float rotation_speed, RigidBodyComponent& rigidbody_comp)
{
  if (target_rotation < -5.f)
  {
    rigidbody_comp.radial_velocity -= rotation_speed;
  }
  else if (target_rotation > 5.f)
  {
    rigidbody_comp.radial_velocity += rotation_speed;
  }
}

void ShootTarget(float target_angle_diff, ShooterComponent& shooter_comp)
{
  if (glm::abs(target_angle_diff) < 20.f)
  {
    shooter_comp.shoot_triggered = true;
  }
  else
  {
    shooter_comp.shoot_triggered = false;
  }
}

void MoveToPos(float delta_time,
  const glm::vec2& target_postion,
  float keep_distance,
  EnemyComponent& enemy_comp,
  RigidBodyComponent& rigidbody_comp,
  TransformComponent& transform_comp)
{
  const glm::vec2 dir_to_target = target_postion - transform_comp.world_transform.position;
  float target_angle_diff = TargetAngleDiff(enemy_comp.target_positin, transform_comp.world_transform.position, transform_comp.world_transform.GetRotation());

  if (glm::length(dir_to_target) > keep_distance && abs(target_angle_diff) < 25.f)
  {
    rigidbody_comp.velocity += transform_comp.world_transform.GetForwardVector() * enemy_comp.acceleration * delta_time;
  }

  // Turn to enemy 
  RotateToTarget(target_angle_diff, enemy_comp.rotation_speed, rigidbody_comp);
}

AIState GetRandomAction()
{
  AIState rand_state = static_cast<AIState>(rand() % int(AIState::COUNT));

  if (rand_state == AIState::IDLE)
  {
    rand_state = AIState::SHOOT;
  }

  return rand_state;
}

void EnemyMoveSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();

  auto view = reg.view<EnemyComponent, RigidBodyComponent, TransformComponent>();

  view.each([delta_time](Entity entity, EnemyComponent& enemy_comp, RigidBodyComponent& rigidbody_comp, TransformComponent& transform_comp) {

    if (GameUtils::GetTime() - enemy_comp.last_reaction_time < enemy_comp.reaction_delay)
    {
      return;
    }

    if (enemy_comp.enemy_type == EnemyType::ASTEROID_BIG ||
      enemy_comp.enemy_type == EnemyType::ASTEROID_MED ||
      enemy_comp.enemy_type == EnemyType::ASTEROID_SMALL)
    {
      transform_comp.world_transform.SetRotation(transform_comp.world_transform.GetRotation() + enemy_comp.rotation_speed * delta_time);
      return;
    }

    bool action_possible = UpdateActionPossible(enemy_comp);

    switch (enemy_comp.ai_state)
    {
    case AIState::IDLE:
    {
      // Don't move, stay still, rotate sometimes
      if (action_possible)
      {
        float target_angle_diff = GameUtils::AnglesDiff(enemy_comp.target_angle, transform_comp.world_transform.GetRotation());
        RotateToTarget(target_angle_diff, enemy_comp.rotation_speed, rigidbody_comp);
      }
      else
      {
        enemy_comp.target_angle = GameUtils::Random(360.f);
        enemy_comp.next_ai_state = GetRandomAction();
      }

      break;
    }
    case AIState::SEEK:
    {
      // Get player pos
      Entity root_entity = Game::GetRootEntity();
      auto& game_state_sc = root_entity.GetComponent<GameStateSingleComponent>();
      enemy_comp.target_positin = game_state_sc.player_pos;

      if (action_possible)
      {
        // Seek player, shoot him
        float target_angle_diff = TargetAngleDiff(enemy_comp.target_positin, transform_comp.world_transform.position, transform_comp.world_transform.GetRotation());
        float keep_distance = enemy_comp.enemy_type == EnemyType::KAMIKADZE ? 0.f : 100.f;

        MoveToPos(delta_time, enemy_comp.target_positin, keep_distance, enemy_comp, rigidbody_comp, transform_comp);

        if (entity.HasComponent<ShooterComponent>())
        {
          ShootTarget(target_angle_diff, entity.GetComponent<ShooterComponent>());
        }
      }
      else
      {
        enemy_comp.next_ai_state = GetRandomAction();
      }

      break;
    }
    case AIState::SHOOT:
    {
      // Get player pos
      Entity root_entity = Game::GetRootEntity();
      auto& game_state_sc = root_entity.GetComponent<GameStateSingleComponent>();
      enemy_comp.target_positin = game_state_sc.player_pos;

      if (action_possible)
      {
        // Seek player, shoot him
        float target_angle_diff = TargetAngleDiff(enemy_comp.target_positin, transform_comp.world_transform.position, transform_comp.world_transform.GetRotation());

        RotateToTarget(target_angle_diff, enemy_comp.rotation_speed, rigidbody_comp);

        // Shoot without moving
        if (entity.HasComponent<ShooterComponent>())
        {
          ShootTarget(target_angle_diff, entity.GetComponent<ShooterComponent>());
        }
      }
      else
      {
        enemy_comp.next_ai_state = GetRandomAction();
      }

      break;
    }
    case AIState::MOVE:
    {
      // Move to position
      if (action_possible)
      {
        // Seek player, shoot him
        float target_angle_diff = TargetAngleDiff(enemy_comp.target_positin, transform_comp.world_transform.position, transform_comp.world_transform.GetRotation());

        MoveToPos(delta_time, enemy_comp.target_positin, 0.f, enemy_comp, rigidbody_comp, transform_comp);
      }
      else
      {
        enemy_comp.next_ai_state = GetRandomAction();
      }
      break;
    }
    default:
      break;
    }

    // On State Changed
    if (action_possible && enemy_comp.next_ai_state != enemy_comp.ai_state)
    {
      switch (enemy_comp.next_ai_state)
      {
      case AIState::IDLE:
        break;
      case AIState::SEEK:
        break;
      case AIState::SHOOT:
        break;
      case AIState::MOVE:
        // Pick random position in radius
        enemy_comp.target_positin = GameUtils::GetRandomSafePos(transform_comp.world_transform.position, glm::vec2(Game::GetWindow().GetWidth(), Game::GetWindow().GetHeight()), 100.f, 400.f);
        break;
      default:
        break;
      }

      if (entity.HasComponent<ShooterComponent>())
      {
        entity.GetComponent<ShooterComponent>().shoot_triggered = false;
      }

      if (enemy_comp.enemy_type == EnemyType::UFO)
      {
        enemy_comp.ai_state = AIState::MOVE;
      }
      else if (enemy_comp.enemy_type == EnemyType::KAMIKADZE)
      {
        enemy_comp.ai_state = AIState::SEEK;
      }
      else
      {
        enemy_comp.ai_state = enemy_comp.next_ai_state;
      }

      enemy_comp.last_state_change_time = GameUtils::GetTime();
    }

    enemy_comp.last_reaction_time = GameUtils::GetTime();
  });
}
