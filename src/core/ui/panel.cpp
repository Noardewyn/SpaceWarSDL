#include "panel.h"

namespace ui
{
void Panel::AddElement(const std::string& name, std::shared_ptr<UIElement> elem)
{
  ui_elements[name] = elem;
}

void Panel::Render()
{
  for (auto elem : ui_elements)
  {
    elem.second->Render();
  }
}
}
