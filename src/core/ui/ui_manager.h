#pragma once

#include "panel.h"

#include <vector>

namespace ui
{
class UIManager
{
public:
  UIManager() = default;
  ~UIManager() = default;

  Panel& CreatePanel();
  uint32_t AddPanel(const Panel& panel);
  uint32_t GetCurrentPanelId() const;
  Panel& GetCurrentPanel();
  const Panel& GetCurrentPanel() const;

  Panel& SetPanel(uint32_t panel_id);
  Panel& GetPanel(uint32_t panel_id);

  void RenderCurrentPanel();
  void Clear();

private:
  uint32_t current_panel_id = 0;
  std::vector<Panel> panels;
};
}
