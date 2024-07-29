#include "texture.h"
#include "core/log.h"

Texture::Texture(SDL_Renderer* renderer, const std::string& path)
  : image_path(path)
{
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());

  if (!loaded_surface)
  {
    LOG_ERROR("Unable to load image {}! SDL_image Error: {}\n", path.c_str(), IMG_GetError());
  }
  else
  {
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));

    if (!CreateTexture(renderer, loaded_surface))
    {
      LOG_ERROR("Unable to create texture from {}! SDL Error: {}\n", path.c_str(), SDL_GetError());
    }
    else
    {
      SDL_FreeSurface(loaded_surface);
    }
  }
}

Texture::Texture(SDL_Renderer* renderer, SDL_Surface* const surface)
{
  assert(surface != nullptr);

  if (!CreateTexture(renderer, surface))
  {
    LOG_ERROR("Unable to create texture from surface! SDL Error: {}\n", SDL_GetError());
  }
}

Texture::Texture(Texture&& other) noexcept
  : Texture(other)
{
  FreeTexture();
  std::swap(texture, other.texture);
}

bool Texture::CreateTexture(SDL_Renderer* renderer, SDL_Surface* const surface)
{
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  if (!texture)
  {
    return false;
  }

  width = surface->w;
  height = surface->h;

  return true;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
  FreeTexture();

  operator=(other);

  other.texture = nullptr;

  return *this;
}

Texture::~Texture()
{
  FreeTexture();
}

void Texture::Render(SDL_Renderer* renderer, const glm::vec2& pos, const RenderParams& params)
{
  assert(renderer != nullptr);

  SDL_Rect render_rect = GetSDLRect(pos, params);

  SDL_SetTextureColorMod(texture, params.color.r, params.color.g, params.color.b);
  SDL_SetTextureAlphaMod(texture, params.color.a);
  SDL_SetTextureBlendMode(texture, params.blend);

  SDL_RenderCopyEx(
    renderer,
    texture,
    params.clip.has_value() ? &params.clip.value() : nullptr,
    &render_rect,
    params.angle,
    params.center.has_value() ? &params.center.value() : nullptr,
    params.flip);
}

const int Texture::GetWidth() const
{
  return width;
}

const int Texture::GetHeight() const
{
  return height;
}

SDL_Rect Texture::GetSDLRect(const glm::vec2& pos, const RenderParams& params) const
{
  SDL_Rect texture_rect{ 0, 0, width, height };

  if (params.clip.has_value())
  {
    texture_rect.w = params.clip->w;
    texture_rect.h = params.clip->h;
  }

  SDL_Rect render_rect;

  if (params.center.has_value())
  {
    render_rect.x = static_cast<int>(pos.x);
    render_rect.y = static_cast<int>(pos.y);
    render_rect.w = static_cast<int>(texture_rect.w);
    render_rect.h = static_cast<int>(texture_rect.h);
  }
  else
  {
    render_rect.x = static_cast<int>(pos.x - texture_rect.w * params.scale.x * 0.5f);
    render_rect.y = static_cast<int>(pos.y - texture_rect.h * params.scale.y * 0.5f);
    render_rect.w = static_cast<int>(texture_rect.w * params.scale.x);
    render_rect.h = static_cast<int>(texture_rect.h * params.scale.y);
  }

  return render_rect;
}

SDL_Texture* Texture::GetSDLTexture() const
{
  return texture;
}

void Texture::FreeTexture()
{
  if (texture != nullptr)
  {
    SDL_DestroyTexture(texture);
    texture = nullptr;
  }
}
