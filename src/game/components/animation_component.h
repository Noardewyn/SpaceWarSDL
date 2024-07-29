#pragma once

#include <vector>

struct AnimationComponent
{
  struct AnimationParams
  {
    std::vector<std::string> frames;
    float animation_rate = 10;
    bool loop = false;
    bool auto_delete = true;
  };

  AnimationParams params;

  int current_frame_id = 0;
  float loop_start_time = 0.0f;
  bool stopped = false;
  bool started = false;

  AnimationComponent(const AnimationParams& params = {})
    : params(params)
  {
  }
};