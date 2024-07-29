#pragma once
#include "entity_pool_base.h"

class ProjectilesPool final : public EntityPoolBase
{
public:
  ~ProjectilesPool() override = default;

protected:
  Entity OnCreate() override;
  void OnReset(Entity entity) override;
};
