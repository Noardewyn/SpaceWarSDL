#pragma once

struct HealthComponent
{
  float health = 1.f;

  HealthComponent(float health)
    : health(health)
  {
  }
};