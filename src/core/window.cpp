#include "window.h"
#include "core/log.h"

#include <SDL.h>
#include <SDL_Image.h>
#include <glad/glad.h>

Window::Window(const uint32_t width, const uint32_t height, const std::string& title)
  : width(width), height(height)
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
  {
    LOG_CRITICAL("SDL could not initialize! SDL_Error: {}\n", SDL_GetError());
  }

  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window == nullptr)
  {
    LOG_CRITICAL("Window could not be created! SDL_Error: {}\n", SDL_GetError());
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  InitGLContext();
}

Window::~Window()
{
  SDL_GL_DeleteContext(window);
  SDL_DestroyWindow(window);
  window = nullptr;

  IMG_Quit();
  SDL_Quit();
}

uint32_t Window::GetWidth() const
{
  return width;
}

uint32_t Window::GetHeight() const
{
  return height;
}

void Window::SetWidth(uint32_t width)
{
  glViewport(0, 0, width, height);
}

void Window::SetHeight(uint32_t height)
{
  glViewport(0, 0, width, height);
}

bool Window::IsFullscreen() const
{
  return fullscreen_mode;
}

void Window::SetFullscreen(bool state)
{
  if (fullscreen_mode && !state)
  {
    SDL_SetWindowFullscreen(window, 0);
  }
  else
  {
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
  }

  fullscreen_mode = state;
}

SDL_Window* const Window::GetSDLWindow() const
{
  return window;
}

void Window::InitGLContext()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_CreateContext(window);
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
  {
    LOG_CRITICAL("Failed to load GL context: {}\n", SDL_GetError());
  }
}
