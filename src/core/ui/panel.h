#pragma once

#include "core/texture.h"
#include "ui_element.h"

#include <memory>
#include <unordered_map>

namespace ui
{
class Panel
{
public:
  Panel() = default;
  ~Panel() = default;

  void AddElement(const std::string& name, std::shared_ptr<UIElement> elem);

  template <typename T = UIElement>
  std::shared_ptr<T> GetElement(const std::string& name)
  {
    assert(ui_elements.find(name) != ui_elements.end());
    return std::dynamic_pointer_cast<T>(ui_elements[name]);
  }

  void Render();

private:
  std::unordered_map<std::string, std::shared_ptr<UIElement>> ui_elements;
};
}
