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

  // Better use quadtree
  view.each([&view](Entity entity,
    CircleCollisionShapeComponent& collision_comp,
    CollisionHandlerComponent& handler_comp,
    TransformComponent& transform_comp) {

    handler_comp.collided_entities.clear();

    if (entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    const glm::vec2& current_position = transform_comp.world_transform.position;
    const float current_radius = collision_comp.radius;

    view.each([&entity, &handler_comp, &current_position, current_radius](Entity other_entity,
      CircleCollisionShapeComponent& other_collision_comp,
      CollisionHandlerComponent& other_handler_comp,
      TransformComponent& other_transform_comp) {

      if (entity == other_entity ||
        !(handler_comp.collision_mask & other_handler_comp.category) ||
        other_entity.HasComponent<InactiveComponent>())
      {
        return;
      }

      const glm::vec2& other_position = other_transform_comp.world_transform.position;
      const float other_radius = other_collision_comp.radius;
      bool res = IsPointInsideCircle(current_position, other_position, current_radius, other_radius);

      if (res)
      {
        handler_comp.collided_entities.push_back(other_entity);
      }
    });
  });
}
