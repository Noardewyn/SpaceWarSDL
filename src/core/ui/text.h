#pragma once

#include "core/texture.h"
#include "ui_element.h"

#include <SDL_ttf.h>

namespace ui
{
class Text : public UIElement
{
public:
  Text(const std::string& path, const std::string& text = "", int font_size = 30, std::optional<SDL_Point> center = std::nullopt, const SDL_Color& color = { 255, 255, 255, 255 });
  ~Text();

  Text(const Text& other) = delete;
  Text& operator=(const Text& other) = delete;

  void SetPos(int x, int y) override;

  const SDL_Rect GetRect() override;

  void SetText(const std::string& text);
  const std::string& GetText() const;

  void Render() override;

private:
  std::string current_text;
  int font_size = 24;

  TTF_Font* font = nullptr;
  Texture texture;
};
}
