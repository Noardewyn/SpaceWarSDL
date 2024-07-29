#pragma once
#include "game_types.h"

struct PoolPropertyComponent
{
  EntityPoolType pool_type;

  PoolPropertyComponent(EntityPoolType pool_type)
    : pool_type(pool_type)
  {

  }
};