#pragma once

struct ShooterComponent
{
  float shoot_delay = 1.0f;
  float projectile_damage = 1.0f;
  float projectile_speed = 0.0f;
  float projectile_lifetime = 0.0f;
  int shots_count = 1;

  // Runtime
  bool shoot_triggered = false;
  float last_shoot_time = 0.0f;

  ShooterComponent() = default;

  ShooterComponent(float shoot_delay, float projectile_damage, float projectile_speed, float projectile_lifetime)
    : shoot_delay(shoot_delay),
    projectile_damage(projectile_damage),
    projectile_speed(projectile_speed),
    projectile_lifetime(projectile_lifetime)
  {
  }
};
