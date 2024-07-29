#pragma once
#include "core/system_base.h"

class DamageSystem : public SystemBase
{
public:
  void Update(float delta_time) override;
};
