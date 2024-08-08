#include "collision_system.h"
#include "core/entity.h"
#include "core/log.h"

#include "components/circle_collision_shape_component.h"
#include "components/transform_component.h"
#include "components/inactive_component.h"
#include "components/collision_handler_component.h"

bool IsPointInsideCircle(const glm::vec2& first, const glm::vec2& second, float first_radius, float second_radius)
{
  return sqrt(pow(second.x - first.x, 2) + pow(second.y - first.y, 2)) <= first_radius + second_radius;
}

void CollisionSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();

  auto view = reg.view<CircleCollisionShapeComponent, CollisionHandlerComponent, TransformComponent>();

  std::vector<std::tuple<Entity, CollisionHandlerComponent*, glm::vec2, float>> collision_bodies;

  // Better use quadtree
  view.each([&collision_bodies](Entity entity,
    CircleCollisionShapeComponent& collision_comp,
    CollisionHandlerComponent& handler_comp,
    TransformComponent& transform_comp) {

    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    handler_comp.collided_entities.clear();

    collision_bodies.emplace_back(entity, &handler_comp, transform_comp.world_transform.position, collision_comp.radius);
  });

  for (size_t i = 0; i < collision_bodies.size(); i++)
  {
    const auto& [first_entity, first_handler, first_pos, first_radius] = collision_bodies[i];

    for (size_t j = i + 1; j < collision_bodies.size(); j++)
    {
      const auto& [second_entity, second_handler, second_pos, second_radius] = collision_bodies[j];

      if (!(first_handler->collision_mask & second_handler->category))
      {
        continue;
      }

      if (IsPointInsideCircle(first_pos, second_pos, first_radius, second_radius))
      {
        first_handler->collided_entities.insert(second_entity);

        if (second_handler->collision_mask & first_handler->category)
        {
          second_handler->collided_entities.insert(first_entity);
        }
      }
    }
  }
}
