#pragma once
#include <glm.hpp>
#include <SDL_pixels.h>

struct SDL_Color;
struct SDL_Renderer;

class GameUtils final
{
public:
  static float Vec2ToAngle(const glm::vec2& vec);
  static glm::vec2 AngleToVec2(float vec);
  static float AnglesDiff(float first, float second);
  static float Remap(float value, float low1, float high1, float low2, float high2);
  static glm::vec2 WrapCoordinates(glm::vec2 src, glm::vec2 border);
  static float WrapAngle(float angle);
  static float RandomNormalized();
  static glm::vec2 GetRandomSafePos(const glm::vec2& position, const glm::vec2& world_size, float min_radius, float max_radius, float border = 50.f);

  static float Random(float max = 1.0f);
  static float Random(float min, float max);
  static float GetTime();

  static void DebugDrawLine(SDL_Renderer* renderer, const glm::vec2& first, const glm::vec2& second, const SDL_Color& color = { 255, 255, 255, 255 });
};
