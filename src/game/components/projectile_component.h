#pragma once

#include "core/entity.h"

#include <glm.hpp>

struct ProjectileComponent
{
  glm::vec2 velocity;
  float lifetime = 0.0f;

  // Runtime
  float spawn_time = 0.0f;

  ProjectileComponent(const glm::vec2& velocity, float lifetime)
    : velocity(velocity),
    lifetime(lifetime)
  {
  }
};
