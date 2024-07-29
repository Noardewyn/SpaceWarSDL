#include "hud_system.h"
#include "game.h"
#include "core/entity.h"
#include "core/ui/text.h"

#include "components/hud_single_component.h"

void HUDSystem::Update(float delta_time)
{
  Entity root_entity = Game::GetRootEntity();

  auto& hud_sc = root_entity.GetComponent<HUDSingleComponent>();

  if (Game::GetUI().GetCurrentPanelId() == (uint32_t)GameScenes::GAME ||
    Game::GetUI().GetCurrentPanelId() == (uint32_t)GameScenes::GAME_OVER)
  {
    const std::string& score_str = "Score: " + std::to_string(hud_sc.score);
    auto& text_element = Game::GetUI().GetCurrentPanel().GetElement<ui::Text>("score_text");

    if (score_str != text_element->GetText())
    {
      text_element->SetText(score_str);
    }

    if (Game::GetUI().GetCurrentPanelId() != (uint32_t)GameScenes::GAME_OVER)
    {
      const std::string& health_str = "Health: " + std::to_string(hud_sc.health);
      auto& health_element = Game::GetUI().GetCurrentPanel().GetElement<ui::Text>("health_text");

      if (health_str != health_element->GetText())
      {
        health_element->SetText(health_str);
      }
    }
  }
}
