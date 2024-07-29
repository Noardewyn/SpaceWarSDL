#pragma once

#include "entity_pool_base.h"

class CollectablesPool final : public EntityPoolBase
{
public:
  ~CollectablesPool() override = default;

private:
  Entity OnCreate() override;
  void OnReset(Entity entity) override;
};
