#pragma once
#include "glm.hpp"

struct InputComponent
{
  glm::vec2 move_direction;
  float steereng = 0.f;
  bool shoot_triggered = false;
};
