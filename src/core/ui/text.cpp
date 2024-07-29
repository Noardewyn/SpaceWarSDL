#include "text.h"
#include "Game.h"
#include "core/log.h"

namespace ui
{
Text::Text(const std::string& path, const std::string& text, int font_size, std::optional<SDL_Point> center, const SDL_Color& color)
  : UIElement({ 0, 0, 0, 0 }, center),
  font_size(font_size)
{
  font = TTF_OpenFont(path.c_str(), font_size);
  if (font == nullptr)
  {
    LOG_ERROR("Failed to load lazy font! SDL_ttf Error: {}\n", TTF_GetError());
  }

  SetText(text);
}

Text::~Text()
{
  TTF_CloseFont(font);
  font = nullptr;
}

void Text::SetPos(int x, int y)
{
  if (center.has_value())
  {
    rect.x = glm::abs(x + Game::GetWindow().GetWidth() * center->x - rect.w * center->x);
    rect.y = glm::abs(y + Game::GetWindow().GetHeight() * center->y - rect.h * center->y);
    return;
  }

  rect.x = x;
  rect.y = y;
}

const SDL_Rect Text::GetRect()
{
  Texture::RenderParams params;
  params.center = center;
  params.scale = scale;

  return rect;
}

void Text::SetText(const std::string& text)
{
  if (current_text == text)
  {
    return;
  }

  current_text = text;

  SDL_Surface* text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
  if (text_surface == nullptr)
  {
    LOG_ERROR("Text: Unable to render text surface! SDL_ttf Error: {}\n", TTF_GetError());
  }
  else
  {
    texture = std::move(Texture(Game::GetRenderer(), text_surface));
    rect.w = texture.GetWidth();
    rect.h = texture.GetHeight();

    SDL_FreeSurface(text_surface);
  }
}

const std::string& Text::GetText() const
{
  return current_text;
}

void Text::Render()
{
  Texture::RenderParams params;
  params.center = center;
  params.color = color;
  params.scale = scale;

  texture.Render(Game::GetRenderer(), glm::vec2(float(rect.x), float(rect.y)), params);
}
}
