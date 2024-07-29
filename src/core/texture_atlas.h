#pragma once
#include <string>
#include <map>
#include <SDL.h>

class TextureAtlas
{
public:
  TextureAtlas(const std::string& path);

  SDL_Rect GetSpriteRect(const std::string& name);

private:
  std::map<std::string, SDL_Rect> frames;
};
