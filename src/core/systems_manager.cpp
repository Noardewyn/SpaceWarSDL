#include "systems_manager.h"

void SystemsManager::Update(float delta_time)
{
  for (auto& system : systems)
  {
    system->Update(delta_time);
  }
}
