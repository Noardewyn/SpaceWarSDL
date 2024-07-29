#pragma once
#include "core/entity.h"

#include <list>

const glm::vec2 pool_objects_position(999999.f, 999999.f);

class EntityPoolBase
{
public:
  virtual ~EntityPoolBase();

  void Init(int pool_size);

  Entity GetEntity();
  void FreeEntity(Entity entity);

  void Reset();

protected:
  virtual Entity OnCreate() = 0;
  virtual void OnReset(Entity entity) = 0;

private:

  int pool_size = 0;
  std::list<Entity> entities;

  Entity CreateEntity();
};
