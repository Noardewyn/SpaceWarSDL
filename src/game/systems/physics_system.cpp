#include "physics_system.h"

#include "game.h"
#include "core/entity.h"
#include "core/log.h"
#include "game_utils.h"

#include "components/transform_component.h"
#include "components/rigid_body_component.h"
#include "components/inactive_component.h"

#include <unordered_map>

const float GRAVITY_CONST = 5000.f;
const float MAX_GRAVITY_FORCE = 350.f;

void PhysicsSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<RigidBodyComponent, TransformComponent>();

  std::vector<std::tuple<Entity, RigidBodyComponent*, float, glm::vec2>> bodies;

  // Get all rigidbodies
  view.each([delta_time, &bodies](Entity entity, RigidBodyComponent& rigid_body_comp, TransformComponent& transform_comp) {
    if (entity.HasComponent<InactiveComponent>() || !Game::HasEntity(entity))
    {
      return;
    }

    if (rigid_body_comp.mass != 0.f)
    {
      bodies.emplace_back(entity, &rigid_body_comp, rigid_body_comp.mass, transform_comp.world_transform.position);
    }
  });

  // Update physic velocity
  for (size_t i = 0; i < bodies.size(); i++)
  {
    const auto& [first_entity, first_body, first_mass, first_pos] = bodies[i];

    for (size_t j = i + 1; j < bodies.size(); j++)
    {
      const auto& [second_entity, second_body, second_mass, second_pos] = bodies[j];

      const glm::vec2& gravitation_dir = second_pos - first_pos;

      if (length(gravitation_dir) == 0.f)
      {
        continue;
      }

      const float distance_to_body = std::max(1.f, glm::length(gravitation_dir));
      const float gravity_force = GRAVITY_CONST * (first_body->mass * second_body->mass) / static_cast<float>(glm::pow(distance_to_body, 2.f));
      const glm::vec2& force_to_apply = glm::normalize(gravitation_dir) * std::min(gravity_force, MAX_GRAVITY_FORCE);

      // Push outside on small distances to prevent stucking inside gravity centers
      const glm::vec2& velocity_delta = force_to_apply * delta_time * (distance_to_body < 50.f ? -2.f : 1.f);

      if (!first_body->lock_position && second_body->ignore_list.find(first_entity.GetName()) == second_body->ignore_list.end())
      {
        first_body->velocity += velocity_delta;
      }

      if (!second_body->lock_position && first_body->ignore_list.find(second_entity.GetName()) == first_body->ignore_list.end())
      {
        second_body->velocity -= velocity_delta;
      }
    }
  }

  // Update transform components
  view.each([delta_time, &bodies](Entity entity, RigidBodyComponent& rigid_body_comp, TransformComponent& transform_comp) {
    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    if (rigid_body_comp.mass != 0 && rigid_body_comp.lock_position == false)
    {
      if (glm::length(rigid_body_comp.velocity) > rigid_body_comp.max_speed)
      {
        rigid_body_comp.velocity = glm::normalize(rigid_body_comp.velocity) * rigid_body_comp.max_speed;
      }

      transform_comp.world_transform.position += rigid_body_comp.velocity * delta_time;
      transform_comp.world_transform.position = GameUtils::WrapCoordinates(transform_comp.world_transform.position, glm::vec2(Game::GetWindow().GetWidth(), Game::GetWindow().GetHeight()));
    }

    rigid_body_comp.radial_velocity *= rigid_body_comp.radial_deceleration_coef;

    if (abs(rigid_body_comp.radial_velocity) > rigid_body_comp.max_radial_speed)
    {
      rigid_body_comp.radial_velocity = glm::sign(rigid_body_comp.radial_velocity) * rigid_body_comp.max_radial_speed;
    }

    const float new_rotation = transform_comp.world_transform.GetRotation() + rigid_body_comp.radial_velocity * delta_time;
    transform_comp.world_transform.SetRotation(new_rotation);
  });
}
