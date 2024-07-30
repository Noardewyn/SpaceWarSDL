#pragma once

#include <glm.hpp>

enum class AIState
{
  IDLE,
  SEEK,
  SHOOT,
  MOVE,
  COUNT
};

struct EnemyComponent
{
  glm::vec2 acceleration = { 0.f, 0.f };
  float max_speed = 1000.f;
  float rotation_speed = 0.0f;
  int level = 0;

  EnemyType enemy_type = EnemyType::ASTEROID_SMALL;
  float reaction_delay = 0.05f;

  // Runtime
  AIState ai_state = AIState::IDLE;
  AIState next_ai_state = AIState::IDLE;

  float last_reaction_time = 0.f;
  float last_state_change_time = 0.f;
  float target_angle = 0.f;
  float last_action_time = 0.f;
  float action_delay = 0.f;
  float action_length = 0.f;

  glm::vec2 target_positin = { 0.f, 0.f };

  CollectableType collectable_to_spawn = CollectableType::NONE;

  EnemyComponent(const EnemyComponent&) = default;

  EnemyComponent(EnemyType enemy_type = EnemyType::ASTEROID_SMALL)
    : enemy_type(enemy_type)
  {
  }

};
