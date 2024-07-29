#pragma once
#include "resources_provider.h"
#include "game_types.h"

#include "core/window.h"
#include "core/ui/ui_manager.h"
#include "core/systems_manager.h"

#include <string>
#include <glm.hpp>
#include <chrono>
#include <entt.hpp>
#include <array>
#include <spdlog/spdlog.h>

struct SDL_Renderer;
class Entity;
class EntityPoolBase;

struct GameParams
{
  std::string window_name = "Game";
  int screen_width = 1600;
  int screen_height = 1200;

  spdlog::level::level_enum log_level = spdlog::level::trace;
};

class Game
{
public:
  Game(GameParams& params);
  ~Game();

  void Run();
  void Close();

  static Window& GetWindow();
  static SDL_Renderer* GetRenderer();
  static entt::registry& GetRegistry();
  static ResourcesProvider& GetResources();
  static std::shared_ptr<EntityPoolBase> GetPool(EntityPoolType pool_type);

  static Entity CreateEntity(const std::string& name = std::string());
  static void DestroyEntity(Entity entity);
  static bool HasEntity(Entity entity);

  static Entity GetRootEntity();

  static ui::Panel& GetHUD();
  static ui::UIManager& GetUI();

  static void ChangeScene(GameScenes scene);

private:
  GameParams params;

  Window window;
  struct SDL_Renderer* renderer = nullptr;
  entt::registry registry;
  entt::entity root_entity;

  ResourcesProvider resourses;
  SystemsManager systems_manager;
  ui::UIManager ui_manager;

  std::array<std::shared_ptr<EntityPoolBase>, uint64_t(EntityPoolType::COUNT)> entity_pools;

  bool running = true;
  bool game_inited = false;

  std::chrono::high_resolution_clock::time_point last_frame_time;

  GameScenes current_scene = GameScenes::NONE;
  GameScenes next_scene = GameScenes::MENU;

  static Game* _instance;

  static Game* GetInstance();

  void InitGUI();
  void InitEntityPools();
  void InitGame();
  void ResetGame();
  void UpdateGame(float delta_time);
  void UpdateMenu(float delta_time);
  void UpdateScene(float delta_time);
  void ProcessEvents();
};