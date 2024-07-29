#pragma once
#include "core/system_base.h"

class PhysicsSystem : public SystemBase
{
public:
  void Update(float delta_time) override;
};
