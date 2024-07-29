#pragma once
#include <stdint.h>
#include <string>

struct SDL_Window;

class Window
{
public:
  Window(const uint32_t width, const uint32_t height, const std::string& title);
  ~Window();

  uint32_t GetWidth() const;
  uint32_t GetHeight() const;

  void SetWidth(uint32_t width);
  void SetHeight(uint32_t height);

  bool IsFullscreen() const;
  void SetFullscreen(bool state);

  SDL_Window* const GetSDLWindow() const;

private:
  uint32_t width = 0;
  uint32_t height = 0;

  bool fullscreen_mode = false;

  SDL_Window* window = nullptr;

  void InitGLContext();
};