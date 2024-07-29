#pragma once

#include <memory>
#include <unordered_map>

#include <SDL_stdinc.h>

namespace tools
{
class InputManager
{
public:
  static void Init();
  static InputManager* const Instance();

  void UpdateState();
  bool KeyPressed(int key_code) const;
  bool KeyDown(int key_code) const;

private:
  static std::unique_ptr<InputManager> _instance;

  int num_keyboard_keys = 0;
  const Uint8* keyboard_states = nullptr; // Internal SDL pointer, cleared by SDL itself.

  mutable std::unordered_map<int, bool> keys_pressed;

  InputManager() = default;
};

} // namespace tools
