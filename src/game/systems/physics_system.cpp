#include "physics_system.h"

#include "game.h"
#include "core/entity.h"
#include "core/log.h"
#include "game_utils.h"

#include "components/transform_component.h"
#include "components/rigid_body_component.h"
#include "components/inactive_component.h"

#include <unordered_map>

const float GRAVITY_CONST = 25000.f;
const float MAX_GRAVITY_FORCE = 350.f;

void PhysicsSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<RigidBodyComponent, TransformComponent>();

  std::unordered_map<entt::entity, std::pair<float, glm::vec2>> centers_of_mass;

  // Get all rigidbodies
  view.each([delta_time, &centers_of_mass](Entity entity, RigidBodyComponent& rigid_body_comp, TransformComponent& transform_comp) {
    if (entity.HasComponent<InactiveComponent>() || !Game::HasEntity(entity))
    {
      return;
    }

    if (rigid_body_comp.mass != 0.f)
    {
      centers_of_mass[entity] = { rigid_body_comp.mass, transform_comp.world_transform.position };
    }
  });

  // Update physic velocity
  view.each([delta_time, &centers_of_mass](Entity entity, RigidBodyComponent& rigid_body_comp, TransformComponent& transform_comp) {
    if (entity.HasComponent<InactiveComponent>() || !Game::HasEntity(entity))
    {
      return;
    }

    for (const auto mass_center : centers_of_mass)
    {
      if (Entity(mass_center.first).HasComponent<InactiveComponent>() || entity == Entity(mass_center.first))
      {
        continue;
      }

      bool ignore = false;
      for (auto ignored_entity : rigid_body_comp.ignore_list)
      {
        if (Entity(mass_center.first).GetName() == ignored_entity)
        {
          ignore = true;
        }
      }

      if (ignore)
      {
        continue;
      }

      glm::vec2 direction = mass_center.second.second - transform_comp.world_transform.position;

      if (glm::length(direction) == 0.f)
      {
        continue;
      }

      const glm::vec2& direction_normalized = glm::normalize(direction);
      const float distance = std::max(1.0f, glm::length(direction));
      const float gravity_force = GRAVITY_CONST * (rigid_body_comp.mass + mass_center.second.first) / static_cast<float>(glm::pow(distance, 2));
      const glm::vec2& force_to_apply = direction_normalized * std::min(gravity_force, MAX_GRAVITY_FORCE);

      if (rigid_body_comp.mass != 0 && rigid_body_comp.lock_position == false)
      {
        if (distance < 50.f)
        {
          rigid_body_comp.velocity -= force_to_apply * 2.f * delta_time;
        }
        else
        {
          rigid_body_comp.velocity += force_to_apply * delta_time;
        }
      }
    }
  });

  // Update transform components
  view.each([delta_time, &centers_of_mass](Entity entity, RigidBodyComponent& rigid_body_comp, TransformComponent& transform_comp) {
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

    float new_rotation = transform_comp.world_transform.GetRotation() + rigid_body_comp.radial_velocity * delta_time;
    transform_comp.world_transform.SetRotation(new_rotation);
  });
}
