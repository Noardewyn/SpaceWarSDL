#pragma once
#include "glm.hpp"

struct MovementComponent
{
  glm::vec2 acceleration;
  float max_speed = 0.0f;
  float rotation_speed = 0.0f;

  MovementComponent() = default;

  MovementComponent(const glm::vec2& acceleration, float max_speed, float rotation_speed)
    : acceleration(acceleration),
    max_speed(max_speed),
    rotation_speed(rotation_speed)
  {
  }
};