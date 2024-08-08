#pragma once

#include "core/transform.h"

#include <unordered_set>

struct RigidBodyComponent
{
  float mass = 10.f;
  bool lock_position = false;
  glm::vec2 velocity = { 0.f, 0.f };
  float radial_velocity = 0.f;
  float radial_deceleration_coef = 0.83f;

  float max_speed = 400.f;
  float max_radial_speed = 1000.f;

  float max_gravity_force = 3.f;

  std::unordered_set<std::string> ignore_list;

  RigidBodyComponent(float mass, bool lock_position = false, const glm::vec2& velocity = { 0.f, 0.f })
    : mass(mass),
    lock_position(lock_position),
    velocity(velocity)
  {
  }
};