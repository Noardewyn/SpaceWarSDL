#pragma once

struct CircleCollisionShapeComponent
{
  float radius = 0.f;

  CircleCollisionShapeComponent(float radius)
    : radius(radius)
  {
  }
};