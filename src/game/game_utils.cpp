#include "game_utils.h"

#include <chrono>
#include <SDL_render.h>

float GameUtils::Vec2ToAngle(const glm::vec2& vec)
{
  float angle = glm::degrees(glm::atan(vec.x, vec.y));
  return glm::mod((angle + 360.f), 360.f);
}

glm::vec2 GameUtils::AngleToVec2(float angle)
{
  return glm::vec2(sin(glm::radians(angle)), -cos(glm::radians(angle)));
}

float GameUtils::AnglesDiff(float first, float second)
{
  return glm::mod((first - second) + 180.f, 360.f) - 180.f;
}

float GameUtils::Remap(float value, float low1, float high1, float low2, float high2)
{
  return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

glm::vec2 GameUtils::WrapCoordinates(glm::vec2 src, glm::vec2 border)
{
  glm::vec2 dst(src);

  if (src.x < 0.f)
  {
    dst.x = src.x + border.x;
  }
  else if (src.x >= border.x)
  {
    dst.x = src.x - border.x;
  }

  if (src.y < 0.f)
  {
    dst.y = src.y + border.y;
  }
  else if (src.y >= border.y)
  {
    dst.y = src.y - border.y;
  }

  return dst;
}

float GameUtils::WrapAngle(float angle)
{
  angle = fmodf(angle, 360.f);

  if (angle < 0.f)
  {
    angle += 360.f;
  }

  return angle;
}

float GameUtils::RandomNormalized()
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

glm::vec2 GameUtils::GetRandomSafePos(const glm::vec2& position, const glm::vec2& world_size, float radius, float max_radius, float border)
{
  const float random_angle = GameUtils::Random(0.f, 360.f);
  glm::vec2 random_dir(cos(random_angle), sin(random_angle));

  float random_distance = GameUtils::Random(radius, max_radius);
  glm::vec2 random_pos = position + (normalize(random_dir) * random_distance);
  return glm::clamp(random_pos, glm::vec2(border, border), glm::vec2(world_size.x - border, world_size.y - border));
}

float GameUtils::Random(float max)
{
  return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / max));
}

float GameUtils::Random(float min, float max)
{
  return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

float GameUtils::GetTime()
{
  const auto now = std::chrono::high_resolution_clock::now();
  const auto epoch = now.time_since_epoch();
  const auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);

  return seconds.count() / 1000.f;
}

void GameUtils::DebugDrawLine(SDL_Renderer* renderer, const glm::vec2& first, const glm::vec2& second, const SDL_Color& color)
{
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  SDL_RenderDrawLine(renderer, int(first.x), int(first.y), int(second.x), int(second.y));
}
