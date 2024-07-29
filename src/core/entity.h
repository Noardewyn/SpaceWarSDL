#pragma once

#include "game.h"

#include <entt.hpp>

struct TagComponent
{
  std::string tag;
};

class Entity
{
public:
  Entity() = default;
  Entity(entt::entity handle);
  Entity(const Entity& other) = default;

  operator bool() const
  {
    return internal_handle != entt::null;
  }

  operator entt::entity() const
  {
    return internal_handle;
  }

  bool operator==(const Entity& other) const
  {
    return internal_handle == other.internal_handle;
  }

  bool operator!=(const Entity& other) const
  {
    return !(*this == other);
  }

  template<typename T, typename... Args>
  T& AddComponent(Args&&... args)
  {
    assert(!HasComponent<T>());
    T& component = Game::GetRegistry().emplace<T>(internal_handle, std::forward<Args>(args)...);
    return component;
  }

  template<typename T, typename... Args>
  T& GetOrAddComponent(Args&&... args)
  {
    T& component = Game::GetRegistry().emplace_or_replace<T>(internal_handle, std::forward<Args>(args)...);
    return component;
  }

  template<typename T>
  T& GetComponent()
  {
    assert(HasComponent<T>());
    return Game::GetRegistry().get<T>(internal_handle);
  }

  template<typename T>
  bool HasComponent()
  {
    return Game::GetRegistry().any_of<T>(internal_handle);
  }

  template<typename T>
  void RemoveComponent()
  {
    assert(HasComponent<T>());
    Game::GetRegistry().remove<T>(internal_handle);
  }

  const std::string& GetName()
  {
    return GetComponent<TagComponent>().tag;
  }

  void Reset()
  {
    internal_handle = entt::null;
  }

private:
  entt::entity internal_handle{ entt::null };
};
