#include "input_manager.h"

#include <cassert>
#include <SDL.h>

namespace tools
{
std::unique_ptr<InputManager> InputManager::_instance = nullptr;

InputManager* const InputManager::Instance()
{
  assert(_instance && "Input manager must be inited first");
  return _instance.get();
}

void InputManager::Init()
{
  _instance = std::unique_ptr<InputManager>(new InputManager());
}

void InputManager::UpdateState()
{
  keyboard_states = SDL_GetKeyboardState(&num_keyboard_keys);
}

bool InputManager::KeyPressed(int key_code) const
{
  bool key_pressed = KeyDown(key_code);

  if (key_pressed && keys_pressed[key_code] == false)
  {
    keys_pressed[key_code] = true;
    return true;
  }

  return false;
}

bool InputManager::KeyDown(int key_code) const
{
  bool key_down = keyboard_states[key_code];

  if (!key_down)
  {
    keys_pressed[key_code] = false;
  }

  return key_down;
}

} // namespace tools