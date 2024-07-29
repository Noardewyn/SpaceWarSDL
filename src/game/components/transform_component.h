#pragma once

#include "core/transform.h"
#include "core/entity.h"

struct TransformComponent
{
  Transform world_transform;

  TransformComponent() = default;

  TransformComponent(const Transform& world_transform)
    : world_transform(world_transform)
  {
  }
};