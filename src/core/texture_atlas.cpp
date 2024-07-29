#include "texture_atlas.h"
#include "core/log.h"

#define RYML_SINGLE_HDR_DEFINE_NOW
#include <rapidyaml.hpp>

#include <fstream> 

TextureAtlas::TextureAtlas(const std::string& path)
{
  std::ifstream input_file(path);

  if (!input_file.is_open())
  {
    LOG_ERROR("TextureAtlas: Can't open file {}", path.c_str());
  }

  std::string content = std::move(std::string(std::istreambuf_iterator<char>(input_file), std::istreambuf_iterator<char>()));
  ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(content));

  for (const ryml::ConstNodeRef& frame : tree["frames"])
  {
    SDL_Rect rect;
    frame["frame"]["x"] >> rect.x;
    frame["frame"]["y"] >> rect.y;
    frame["frame"]["w"] >> rect.w;
    frame["frame"]["h"] >> rect.h;
    
    std::string key_str;
    frame >> ryml::key(key_str);

    frames.insert({ key_str, rect});
  }

  input_file.close();
}

SDL_Rect TextureAtlas::GetSpriteRect(const std::string& name)
{
  if (auto it = frames.find(name); it != frames.end())
  {
    return it->second;
  }

  LOG_ERROR("TextureAtlas: Can't find sprite with name {}", name.c_str());

  return SDL_Rect();
}
