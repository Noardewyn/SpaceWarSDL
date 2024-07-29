#include "ui_element.h"

namespace ui
{
UIElement::UIElement(const SDL_Rect& rect, std::optional<SDL_Point> center)
  : rect(rect),
  center(center)
{
}

void UIElement::SetPos(int x, int y)
{
  rect.x = x;
  rect.y = y;
}

void UIElement::SetScale(float x, float y)
{
  scale.x = x;
  scale.y = y;
}

void UIElement::SetColor(const SDL_Color& color)
{
  this->color = color;
}

const SDL_Rect UIElement::GetRect()
{
  return rect;
}

void UIElement::SetRect(const SDL_Rect& rect)
{
  this->rect = rect;
}

}
