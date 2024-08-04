#include "player_input_system.h"

#include "game.h"
#include "core/input_manager.h"
#include "components/player_component.h"
#include "components/input_component.h"
#include "core/log.h"

void PlayerInputSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<PlayerComponent, InputComponent>();
  const tools::InputManager* input_manager = tools::InputManager::Instance();

  view.each([input_manager](PlayerComponent& player_comp, InputComponent& input_comp) {

    // Shooting
    input_comp.shoot_triggered = input_manager->KeyDown(SDL_SCANCODE_SPACE);

    // Acceleration
    if (input_manager->KeyDown(SDL_SCANCODE_UP) || input_manager->KeyDown(SDL_SCANCODE_W))
    {
      input_comp.move_direction.y = 1.f;
    }
    else
    {
      input_comp.move_direction.y = 0.f;
    }

    // Steering
    if (input_manager->KeyDown(SDL_SCANCODE_LEFT) || input_manager->KeyDown(SDL_SCANCODE_A))
    {
      input_comp.steereng = -1.f;
    }
    else if (input_manager->KeyDown(SDL_SCANCODE_RIGHT) || input_manager->KeyDown(SDL_SCANCODE_D))
    {
      input_comp.steereng = 1.f;
    }
    else
    {
      input_comp.steereng = 0.f;
    }
  });
}
