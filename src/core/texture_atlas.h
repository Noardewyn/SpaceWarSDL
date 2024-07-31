#pragma once
#include <string>
#include <unordered_map>
#include <SDL.h>

class TextureAtlas
{
public:
  TextureAtlas(const std::string& path);

  SDL_Rect GetSpriteRect(const std::string& name);

private:
  std::unordered_map<std::string, SDL_Rect> frames;
};
