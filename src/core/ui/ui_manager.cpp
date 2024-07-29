#include "ui_manager.h"

namespace ui
{

Panel& UIManager::CreatePanel()
{
  return panels.emplace_back();
}

uint32_t UIManager::AddPanel(const Panel& panel)
{
  panels.emplace_back(panel);
  return uint32_t(panels.size() - 1);
}

uint32_t UIManager::GetCurrentPanelId() const
{
  return current_panel_id;
}

Panel& UIManager::GetCurrentPanel()
{
  return panels[current_panel_id];
}

const Panel& UIManager::GetCurrentPanel() const
{
  return panels[current_panel_id];
}

Panel& UIManager::SetPanel(uint32_t panel_id)
{
  current_panel_id = panel_id;
  return GetCurrentPanel();
}

ui::Panel& UIManager::GetPanel(uint32_t panel_id)
{
  return panels[panel_id];
}

void UIManager::RenderCurrentPanel()
{
  panels[current_panel_id].Render();
}

void UIManager::Clear()
{
  panels.clear();
}

}
