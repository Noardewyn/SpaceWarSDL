#pragma once

#include "system_base.h"

#include <memory>
#include <vector>

class SystemsManager
{
public:
  template<typename T = SystemBase>
  void Register()
  {
    systems.emplace_back(std::make_unique<T>());
  }

  void Update(float delta_time);

private:
  std::vector<std::unique_ptr<SystemBase>> systems;
};