#pragma once

#include "entity_pool_base.h"

class EffectsPool final : public EntityPoolBase
{
public:
  ~EffectsPool() override = default;

private:
  Entity OnCreate() override;
  void OnReset(Entity entity) override;
};
