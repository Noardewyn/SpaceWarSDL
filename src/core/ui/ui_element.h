#pragma once

#include <glm.hpp>
#include <SDL.h>
#include <optional>

namespace ui
{
const SDL_Point element_pivot_left = { 0, 0 };
const SDL_Point element_pivot_right = { 1, 0 };
const std::nullopt_t element_pivot_center = std::nullopt;

class UIElement
{
public:
  UIElement() = default;
  UIElement(const SDL_Rect& rect, std::optional<SDL_Point> center = element_pivot_center);
  virtual ~UIElement() = default;

  virtual void Render() = 0;

  virtual void SetPos(int x, int y);
  virtual void SetScale(float x, float y);
  virtual void SetColor(const SDL_Color& color);

  virtual const SDL_Rect GetRect();
  virtual void SetRect(const SDL_Rect& rect);

protected:
  SDL_Rect rect = { 0, 0, 0, 0 };
  SDL_Color color = { 255, 255, 255, 255 };
  glm::vec2 scale = { 1.f, 1.f };

  std::optional<SDL_Point> center = element_pivot_center;
};
}
