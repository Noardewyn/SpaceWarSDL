#include "transform.h"
#include "game_utils.h"

#include <gtc/matrix_transform.hpp>

Transform::Transform(float x, float y, float rotation)
  : position(glm::vec2(x, y)),
  rotation(rotation)
{
}

glm::vec2 Transform::GetForwardVector() const
{
  return GameUtils::AngleToVec2(rotation);
}

void Transform::SetRotation(float rot)
{
  rotation = GameUtils::WrapAngle(rot);
}

float Transform::GetRotation() const
{
  return rotation;
}
