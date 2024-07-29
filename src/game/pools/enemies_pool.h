#pragma once
#include "entity_pool_base.h"

class EnemiesPool final : public EntityPoolBase
{
public:
  ~EnemiesPool() override = default;

private:
  Entity OnCreate() override;
  void OnReset(Entity entity) override;
};
