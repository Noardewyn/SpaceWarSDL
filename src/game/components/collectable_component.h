#pragma once
#include "game_types.h"

struct CollectableComponent
{
  CollectableType collectable_type;
  float lifetime = 10.f;
  float spawn_time = 0.f;

  CollectableComponent(CollectableType collectable_type)
    : collectable_type(collectable_type)
  {
  }
};