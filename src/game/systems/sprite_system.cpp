#include "sprite_system.h"

#include "game.h"
#include "game_utils.h"
#include "core/entity.h"
#include "game/pools/entity_pool_base.h"

#include "components/sprite_component.h"
#include "components/transform_component.h"
#include "components/animation_component.h"
#include "components/inactive_component.h"

void SpriteSystem::Update(float delta_time)
{
  entt::registry& reg = Game::GetRegistry();

  // Render sprites
  auto view = reg.view<SpriteComponent, TransformComponent>();

  view.each([](Entity entity, SpriteComponent& sprite_comp, TransformComponent& transform_comp) {
    if (entity.HasComponent<InactiveComponent>() || sprite_comp.current_frame.empty())
    {
      return;
    }

    const glm::vec2& position = transform_comp.world_transform.position;

    Texture::RenderParams params;
    params.angle = transform_comp.world_transform.GetRotation();
    params.scale = transform_comp.world_transform.scale;
    params.clip = sprite_comp.texture_atlas->GetSpriteRect(sprite_comp.current_frame);
    params.color = sprite_comp.color;

    sprite_comp.texture->texture.Render(Game::GetRenderer(), position, params);
  });

  // Update animations
  auto view_anim = reg.view<AnimationComponent, SpriteComponent, TransformComponent>();

  view_anim.each([delta_time](Entity entity, AnimationComponent& anim_comp, SpriteComponent& sprite_comp, TransformComponent& transform_comp) {
    if (anim_comp.stopped || entity.HasComponent<InactiveComponent>() || anim_comp.params.frames.empty())
    {
      return;
    }

    if (!anim_comp.started)
    {
      anim_comp.loop_start_time = GameUtils::GetTime();
      anim_comp.started = true;
    }

    sprite_comp.current_frame = anim_comp.params.frames[anim_comp.current_frame_id];

    int next_frame = int((GameUtils::GetTime() - anim_comp.loop_start_time) * anim_comp.params.animation_rate) % (anim_comp.params.frames.size());

    if (next_frame != anim_comp.current_frame_id && next_frame == 0)
    {
      anim_comp.loop_start_time = GameUtils::GetTime();

      if (!anim_comp.params.loop)
      {
        anim_comp.stopped = true;
        anim_comp.started = false;
        sprite_comp.current_frame = "";

        if (anim_comp.params.auto_delete)
        {
          Game::GetPool(EntityPoolType::EFFECTS)->FreeEntity(entity);
        }
      }
    }

    anim_comp.current_frame_id = next_frame;
  });
}
