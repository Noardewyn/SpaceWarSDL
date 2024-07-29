#pragma once

#include "assets/game_config_asset.h"
#include "core/entity.h"

struct GameStateSingleComponent
{
  Asset<GameConfigAsset> game_config;

  // Runtime
  int enemies_left = 0;
  int current_wave = -1;

  float player_death_time = 0.f;
  float wave_finished_time = 0.f;
  float time_scale = 0.8f;

  Entity player_entity;

  bool player_death = false;
  bool wave_spawning = false;

  float player_rotation = 0.f;
  glm::vec2 player_pos = { 0.f, 0.f };

  GameStateSingleComponent(Asset<GameConfigAsset> game_config)
    : game_config(game_config)
  {

  }
};