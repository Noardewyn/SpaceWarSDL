#pragma once

enum class GameScenes
{
  NONE = -1,
  MENU,
  GAME,
  GAME_OVER,
  COUNT
};

enum class GameState
{
  PREPARATION = 0,
  WAVE_IN_PROGRESS,
  WAVE_COMPLETED,
  GAME_OVER,
  COUNT
};

enum class EntityPoolType
{
  PROJECTILE,
  ENEMY,
  COLLECTABLE,
  EFFECTS,
  COUNT
};

enum class EnemyType
{
  ASTEROID_SMALL,
  ASTEROID_MED,
  ASTEROID_BIG,
  SHOOTER,
  KAMIKADZE,
  UFO,
  COUNT
};

enum class CollectableType
{
  NONE,
  EXTRA_LIFE,
  MASS_DEATH,
  EXTRA_SHOT,
  RAPID_FIRE,
  COUNT
};

enum class EffectType
{
  EXPLOSION,
  PROJECTILE_HIT,
  THRUSTER,
  COUNT
};
