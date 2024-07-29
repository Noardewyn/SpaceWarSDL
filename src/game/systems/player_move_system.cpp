#include "player_move_system.h"

#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"
#include "core/entity.h"
#include "core/input_manager.h"

#include "components/transform_component.h"
#include "components/player_component.h"
#include "components/game_state_single_component.h"
#include "components/rigid_body_component.h"
#include "components/shooter_component.h"

const int THRUSTER_SOUND_CHANNEL = 14;

void PlayerMoveSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<PlayerComponent, RigidBodyComponent, TransformComponent, ShooterComponent>();

  view.each([delta_time](Entity entity, PlayerComponent& player_comp, RigidBodyComponent& rigidbody_comp, TransformComponent& transform_comp, ShooterComponent& shooter_comp) {

    const tools::InputManager* input_manager = tools::InputManager::Instance();

    // Shooting
    shooter_comp.shoot_triggered = input_manager->KeyDown(SDL_SCANCODE_SPACE);

    // Forward movement
    if (input_manager->KeyDown(SDL_SCANCODE_UP) || input_manager->KeyDown(SDL_SCANCODE_W))
    {
      if (!Mix_Playing(THRUSTER_SOUND_CHANNEL))
      {
        Game::GetResources().thruster_sound->Play(THRUSTER_SOUND_CHANNEL);
      }

      if (glm::length(player_comp.velocity) > player_comp.max_speed)
      {
        player_comp.velocity = glm::normalize(player_comp.velocity) * player_comp.max_speed;
      }

      const glm::vec2& direction = transform_comp.world_transform.GetForwardVector();
      rigidbody_comp.velocity += direction * player_comp.acceleration * delta_time;
    }
    else
    {
      if (Mix_Playing(THRUSTER_SOUND_CHANNEL))
      {
        Mix_HaltChannel(THRUSTER_SOUND_CHANNEL);
      }
    }

    // Steering
    if (input_manager->KeyDown(SDL_SCANCODE_LEFT) || input_manager->KeyDown(SDL_SCANCODE_A))
    {
      rigidbody_comp.radial_velocity -= player_comp.rotation_speed;
    }
    else if (input_manager->KeyDown(SDL_SCANCODE_RIGHT) || input_manager->KeyDown(SDL_SCANCODE_D))
    {
      rigidbody_comp.radial_velocity += player_comp.rotation_speed;
    }

    if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
    {
      Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_pos = transform_comp.world_transform.position;
      Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_rotation = transform_comp.world_transform.GetRotation();
    }
  });
}
