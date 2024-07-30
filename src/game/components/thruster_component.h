#pragma once

#include <glm.hpp>
#include <entt.hpp>

struct ThrusterComponent
{
  glm::vec2 offset;

  // Runtime
  bool turn_on = false;
  entt::entity effect_entity = entt::null;

  ThrusterComponent() = default;

  ThrusterComponent(const glm::vec2& offset)
    : offset(offset)
  {
  }
};
