#pragma once

struct DamageDealerComponent
{
  Entity owner;
  float damage = 1.f;
  bool remove_after_damage = true; // Delete entity after damage done

  DamageDealerComponent(float damage, Entity owner, bool remove_after_damage = false)
    : damage(damage),
    owner(owner),
    remove_after_damage(remove_after_damage)
  {
  }
};
