#pragma once

// Marks entity as dead entity. Dead entity will be deleted soon
struct DeadComponent
{
  bool is_crash_damage = false;

  DeadComponent(bool is_crash_damage)
    : is_crash_damage(is_crash_damage)
  {

  }
};