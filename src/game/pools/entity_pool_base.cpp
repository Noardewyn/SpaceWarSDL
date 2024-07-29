#include "entity_pool_base.h"
#include "game.h"

#include "components/inactive_component.h"
#include "components/pool_property_component.h"
#include "core/log.h"

EntityPoolBase::~EntityPoolBase()
{
  Reset();
}

Entity EntityPoolBase::CreateEntity()
{
  Entity new_entity = OnCreate();

  new_entity.GetOrAddComponent<InactiveComponent>();

  entities.emplace_back(new_entity);

  pool_size++;

  return entities.back();
}

void EntityPoolBase::Init(int initial_size)
{
  for (int i = 0; i < initial_size; i++)
  {
    CreateEntity();
  }
}

Entity EntityPoolBase::GetEntity()
{
  Entity free_entity;

  if (entities.empty())
  {
    free_entity = CreateEntity();
  }
  else
  {
    free_entity = entities.front();
  }

  if (free_entity.HasComponent<InactiveComponent>())
  {
    free_entity.RemoveComponent<InactiveComponent>();
  }

  entities.pop_front();

  return free_entity;
}

void EntityPoolBase::FreeEntity(Entity entity)
{
  OnReset(entity);

  entity.GetOrAddComponent<InactiveComponent>();

  entities.emplace_back(entity);
}

void EntityPoolBase::Reset()
{
  for (Entity& entity : entities)
  {
    Game::DestroyEntity(entity);
  }

  entities.clear();
}
