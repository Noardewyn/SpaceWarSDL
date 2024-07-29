#pragma once

struct PlayerComponent
{
  glm::vec2 velocity;
  glm::vec2 acceleration;
  float max_speed = 0.0f;
  float rotation_speed = 0.0f;

  PlayerComponent(const PlayerComponent&) = default;

  PlayerComponent(const glm::vec2& acceleration, float max_speed, float rotation_speed)
    : velocity(glm::vec2(0.0f, 0.0f)),
    acceleration(acceleration),
    max_speed(max_speed),
    rotation_speed(rotation_speed)
  {
  }

};
