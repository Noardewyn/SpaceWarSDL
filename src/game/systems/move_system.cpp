#include "move_system.h"

#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"
#include "core/entity.h"

#include "components/transform_component.h"
#include "components/player_component.h"
#include "components/game_state_single_component.h"
#include "components/rigid_body_component.h"
#include "components/thruster_component.h"
#include "components/death_component.h"
#include "components/input_component.h"
#include "components/movement_component.h"

void MoveSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<MovementComponent>();

  view.each([delta_time](Entity entity, MovementComponent& movement_comp) {

    const bool is_player = entity.HasComponent<PlayerComponent>();
    const bool is_dead = entity.HasComponent<DeadComponent>();
    const auto& input_component = entity.GetComponent<InputComponent>();
    const auto& transform_comp = entity.GetComponent<TransformComponent>();
    auto& rigidbody_comp = entity.GetComponent<RigidBodyComponent>();

    // Forward movement
    if (!is_dead && input_component.move_direction.y > 0.f)
    {
      if (is_player && !Mix_Playing(THRUSTER_SOUND_CHANNEL))
      {
        Game::GetResources().thruster_sound->Play(THRUSTER_SOUND_CHANNEL);
      }

      if (entity.HasComponent<ThrusterComponent>())
      {
        auto& thruster_comp = entity.GetComponent<ThrusterComponent>();
        thruster_comp.turn_on = true;
      }

      const glm::vec2& direction = transform_comp.world_transform.GetForwardVector();
      rigidbody_comp.velocity += input_component.move_direction.y * direction * movement_comp.acceleration * delta_time;
    }
    else
    {
      if (is_player && Mix_Playing(THRUSTER_SOUND_CHANNEL))
      {
        Mix_HaltChannel(THRUSTER_SOUND_CHANNEL);
      }

      if (entity.HasComponent<ThrusterComponent>())
      {
        auto& thruster_comp = entity.GetComponent<ThrusterComponent>();
        thruster_comp.turn_on = false;
      }
    }

    if (!is_dead)
    {
      // Steering
      rigidbody_comp.radial_velocity += input_component.steereng * movement_comp.rotation_speed;

      if (is_player && Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
      {
        Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_pos = transform_comp.world_transform.position;
        Game::GetRootEntity().GetComponent<GameStateSingleComponent>().player_rotation = transform_comp.world_transform.GetRotation();
      }
    }
  });
}
