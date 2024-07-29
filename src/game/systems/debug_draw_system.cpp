#include "debug_draw_system.h"
#include "game.h"
#include "core/entity.h"

#include "components/circle_collision_shape_component.h"
#include "components/transform_component.h"
#include "components/inactive_component.h"

#include <SDL.h>

void DrawCircle(SDL_Renderer* renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
  const int32_t diameter = (radius * 2);

  int32_t x = (radius - 1);
  int32_t y = 0;
  int32_t tx = 1;
  int32_t ty = 1;
  int32_t error = (tx - diameter);

  while (x >= y)
  {
    SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
    SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
    SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

    if (error <= 0)
    {
      ++y;
      error += ty;
      ty += 2;
    }

    if (error > 0)
    {
      --x;
      tx += 2;
      error += (tx - diameter);
    }
  }
}

void DebugDrawSystem::Update(float delta_time)
{
  bool draw_colliders = false;

  entt::registry& reg = Game::GetRegistry();
  auto view = reg.view<CircleCollisionShapeComponent, TransformComponent>();

  view.each([draw_colliders](Entity current_entity, CircleCollisionShapeComponent& collision_comp, TransformComponent& transform_comp) {
    if (current_entity.HasComponent<InactiveComponent>())
    {
      return;
    }

    const glm::vec2& current_position = transform_comp.world_transform.position;
    const float current_radius = collision_comp.radius;

    if (draw_colliders)
    {
      SDL_SetRenderDrawColor(Game::GetRenderer(), 0, 255, 0, 255);
      DrawCircle(Game::GetRenderer(), (int32_t)current_position.x, (int32_t)current_position.y, (int32_t)current_radius);
    }
  });
}
