#pragma once

#include <glm.hpp>

struct Transform
{
  glm::vec2 position = glm::vec2(0.0f);
  glm::vec2 scale = glm::vec2(1.0f);

  glm::vec2 up = glm::vec2(0.0f, 1.0f);
  glm::vec2 right = glm::vec2(1.0f, 0.0f);

  Transform() = default;
  Transform(float x, float y, float rotation = 0.0f);

  glm::vec2 GetForwardVector() const;

  void SetRotation(float rot);
  float GetRotation() const;

private:
  float rotation = 0.0f;
};
