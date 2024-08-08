#pragma once
#include <unordered_set>

enum CollisionMask
{
  PLAYER = (1u << 0),
  ENEMY = (1u << 1),
  PROJECTILE = (1u << 2),
  COLLECTABLE = (1u << 3),

  ALL = 0xFFFFFFFF
};

// Component for storing information about current collisions
struct CollisionHandlerComponent
{
  CollisionMask category = CollisionMask::PLAYER;
  CollisionMask collision_mask = CollisionMask::ALL;

  // Runtime
  std::unordered_set<entt::entity> collided_entities;

  CollisionHandlerComponent(CollisionMask category, CollisionMask collision_mask)
    : category(category),
    collision_mask(collision_mask)
  {
  }
};
