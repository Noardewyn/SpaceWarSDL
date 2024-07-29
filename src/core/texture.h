#pragma once

#include <string>
#include <optional>

#include <glm.hpp>
#include <SDL.h>
#include <SDL_Image.h>

class Texture
{
public:
  struct RenderParams
  {
    float angle = 0.f;
    glm::vec2 scale = { 1.f, 1.f };

    std::optional<SDL_Rect> clip = std::nullopt;
    std::optional<SDL_Point> center = std::nullopt; // Center of the texture by default. {0, 0} for left upper corner
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_BlendMode blend = SDL_BLENDMODE_BLEND;
    SDL_RendererFlip flip = SDL_FLIP_NONE;
  };

  Texture() = default;
  ~Texture();

  Texture(SDL_Renderer* renderer, const std::string& path);
  Texture(SDL_Renderer* renderer, SDL_Surface* const surface);

  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;

  void Render(SDL_Renderer* renderer, const glm::vec2& pos, const RenderParams& params = RenderParams());

  const int GetWidth() const;
  const int GetHeight() const;

  SDL_Rect GetSDLRect(const glm::vec2& pos, const RenderParams& params = RenderParams()) const;
  SDL_Texture* GetSDLTexture() const;

private:
  std::string image_path;

  SDL_Texture* texture = nullptr;

  int width = 0;
  int height = 0;

  Texture(const Texture& other) = default;
  Texture& operator=(const Texture& other) = default;

  bool CreateTexture(SDL_Renderer* renderer, SDL_Surface* const surface);
  void FreeTexture();
};
