#include "game.h"
#include "game_utils.h"
#include "gameplay_statics.h"

#include "assets/sound_asset.h"

#include "core/ui/text.h"
#include "core/asset_manager.h"
#include "core/ui/panel.h"
#include "core/input_manager.h"
#include "core/texture.h"
#include "core/log.h"
#include "core/window.h"
#include "core/entity.h"

#include "pools/projectiles_pool.h"
#include "pools/enemies_pool.h"
#include "pools/collectables_pool.h"
#include "pools/effects_pool.h"

#include "components/hud_single_component.h"
#include "components/game_state_single_component.h"
#include "systems/shoot_system.h"
#include "systems/collision_system.h"
#include "systems/move_system.h"
#include "systems/enemy_behavior_system.h"
#include "systems/projectile_system.h"
#include "systems/physics_system.h"
#include "systems/collectable_system.h"
#include "systems/damage_system.h"
#include "systems/death_system.h"
#include "systems/sprite_system.h"
#include "systems/debug_draw_system.h"
#include "systems/hud_system.h"
#include "systems/game_state_system.h"
#include "systems/thruster_system.h"
#include "systems/player_input_system.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_Image.h>
#include <SDL_ttf.h>
#include <glad/glad.h>
#include <array>

Game* Game::_instance = nullptr;

Game::Game(GameParams& params)
  : params(params),
  window(params.screen_width, params.screen_height, params.window_name)
{
  tools::Logger::Init(params.log_level);

  LOG_INFO("Init Game...");

  _instance = this;

  srand(static_cast<unsigned>(time(0)));

  renderer = SDL_CreateRenderer(window.GetSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer)
  {
    LOG_CRITICAL("Renderer could not be created! SDL Error: {}\n", SDL_GetError());
    running = false;
    return;
  }

  SDL_RenderSetLogicalSize(renderer, params.screen_width, params.screen_height);

  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
  {
    LOG_CRITICAL("SDL_image could not initialize! SDL_image Error: {}\n", IMG_GetError());
    running = false;
  }

  if (TTF_Init() == -1)
  {
    LOG_CRITICAL("SDL_ttf could not initialize! SDL_ttf Error: {}\n", TTF_GetError());
    running = false;
    return;
  }

  //Initialize SDL_mixer
  const int SOUND_FREQUENCY = 44100;
  const int SAMPLE_SIZE = 2048;
  const int CHANNELS_COUNT = 2;

  if (Mix_OpenAudio(SOUND_FREQUENCY, MIX_DEFAULT_FORMAT, CHANNELS_COUNT, SAMPLE_SIZE) < 0)
  {
    LOG_ERROR("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
  }

  // Pre allocate sound channels
  Mix_AllocateChannels(15);

  tools::InputManager::Init();
  AssetManager::SetAssetsPath("res/");

  last_frame_time = std::chrono::steady_clock::now();
}

Game::~Game()
{
  Close();
}

Game* Game::GetInstance()
{
  return _instance;
}

SDL_Renderer* Game::GetRenderer()
{
  return GetInstance()->renderer;
}

Window& Game::GetWindow()
{
  return GetInstance()->window;
}

entt::registry& Game::GetRegistry()
{
  return GetInstance()->registry;
}

ResourcesProvider& Game::GetResources()
{
  return GetInstance()->resourses;
}

std::shared_ptr<EntityPoolBase> Game::GetPool(EntityPoolType pool_type)
{
  assert(pool_type < EntityPoolType::COUNT);
  return GetInstance()->entity_pools[uint64_t(pool_type)];
}

void Game::InitGame()
{
  if (game_inited)
  {
    return;
  }

  ResetGame();

  root_entity = Game::CreateEntity("Root");
  Entity(root_entity).AddComponent<HUDSingleComponent>();
  Entity(root_entity).AddComponent<GameStateSingleComponent>(Game::GetResources().game_config_asset);

  GameplayStatics::SpawnSun();
  GameplayStatics::SpawnPlayer();

  InitEntityPools();

  game_inited = true;
}

void Game::InitGUI()
{
  uint32_t screen_width = Game::GetWindow().GetWidth();
  uint32_t screen_height = Game::GetWindow().GetHeight();
  int half_width = static_cast<int>(screen_width / 2);
  int half_height = static_cast<int>(screen_height / 2);

  std::string regular_font_path = "res/fonts/Kenney Future.ttf";
  std::string narrow_font_path = "res/fonts/Kenney Future Narrow.ttf";

  // Main menu
  {
    ui::Panel& menu_panel = ui_manager.CreatePanel();
    auto title_text = std::make_shared<ui::Text>(regular_font_path, "SPACE WAR", 200);
    title_text->SetPos(half_width, 150);

    auto start_game_text = std::make_shared<ui::Text>(narrow_font_path, "Start Game", 100);
    start_game_text->SetPos(half_width, half_height - 100);

    auto exit_game_text = std::make_shared<ui::Text>(narrow_font_path, "Exit Game", 100);
    exit_game_text->SetPos(half_width, half_height + 50);

    menu_panel.AddElement("title", title_text);
    menu_panel.AddElement("start_game_text", start_game_text);
    menu_panel.AddElement("exit_game_text", exit_game_text);

    auto controls_text = std::make_shared<ui::Text>(narrow_font_path, "Controls:", 80, ui::element_pivot_left);
    controls_text->SetPos(half_width + 50, half_height + 180);

    auto controls_text1 = std::make_shared<ui::Text>(narrow_font_path, "WASD/ARROWS: movement", 50, ui::element_pivot_left);
    controls_text1->SetPos(half_width + 50, half_height + 250);

    auto controls_text2 = std::make_shared<ui::Text>(narrow_font_path, "SPACE: fire", 50, ui::element_pivot_left);
    controls_text2->SetPos(half_width + 50, half_height + 300);

    auto controls_text3 = std::make_shared<ui::Text>(narrow_font_path, "F10: fullscreen", 50, ui::element_pivot_left);
    controls_text3->SetPos(half_width + 50, half_height + 350);

    menu_panel.AddElement("controls_text", controls_text);
    menu_panel.AddElement("controls_text1", controls_text1);
    menu_panel.AddElement("controls_text2", controls_text2);
    menu_panel.AddElement("controls_text3", controls_text3);
  }

  // GUI
  {
    ui::Panel& menu_panel = ui_manager.CreatePanel();
    auto score_text = std::make_shared<ui::Text>(narrow_font_path, "Score: 0", 50, ui::element_pivot_left);
    score_text->SetPos(20, 10);

    auto health_text = std::make_shared<ui::Text>(narrow_font_path, "Health: 0", 50, ui::element_pivot_right);
    health_text->SetPos(0, 10);

    menu_panel.AddElement("score_text", score_text);
    menu_panel.AddElement("health_text", health_text);
  }

  // Game Over screen
  {
    ui::Panel& menu_panel = ui_manager.CreatePanel();
    auto game_over_text = std::make_shared<ui::Text>(narrow_font_path, "Game Over!", 150);
    game_over_text->SetPos(half_width, half_height - 100);

    auto score_text = std::make_shared<ui::Text>(narrow_font_path, "Score: 0", 100);
    score_text->SetPos(half_width, half_height + 100);

    menu_panel.AddElement("game_over_text", game_over_text);
    menu_panel.AddElement("score_text", score_text);
  }
}

void Game::UpdateMenu(float delta_time)
{
  ui_manager.SetPanel(uint32_t(current_scene));

  auto input_manager = tools::InputManager::Instance();
  input_manager->UpdateState();

  static int active_button = 0;
  static int selected_button = 0;
  static int buttons_count = 2;
  const std::string button_names[2] = { "start_game_text", "exit_game_text" };

  if (current_scene == GameScenes::MENU)
  {
    // Draw background
    SDL_RenderCopy(renderer, Game::GetResources().menu_texture_asset->texture.GetSDLTexture(), nullptr, nullptr);

    if (selected_button != active_button)
    {
      auto active_button_text = ui_manager.GetCurrentPanel().GetElement(button_names[active_button]);
      active_button_text->SetScale(1.f, 1.f);
      active_button_text->SetColor({ 255, 255, 255, 255 });

      active_button = selected_button;
    }

    // Animation for active menu button
    auto active_button_text = ui_manager.GetCurrentPanel().GetElement(button_names[active_button]);

    float animation_speed = 3.f;
    float scale = GameUtils::Remap(cos(GameUtils::GetTime() * animation_speed), -1.f, 1.f, 0.95f, 1.1f);
    float color = GameUtils::Remap(cos(GameUtils::GetTime() * animation_speed), -1.f, 1.f, 0.7f, 1.f);

    active_button_text->SetScale(scale, scale);
    active_button_text->SetColor({ unsigned char(color * 255), unsigned char(color * 165), 0, 255 });

    if (input_manager->KeyPressed(SDL_SCANCODE_RETURN) || input_manager->KeyPressed(SDL_SCANCODE_SPACE))
    {
      if (selected_button == 0)
      {
        ChangeScene(GameScenes::GAME);
      }
      else if (selected_button == 1)
      {
        running = false;
      }
    }
    else if (input_manager->KeyPressed(SDL_SCANCODE_W) || input_manager->KeyPressed(SDL_SCANCODE_UP))
    {
      selected_button = abs(selected_button - 1) % buttons_count;
    }
    else if (input_manager->KeyPressed(SDL_SCANCODE_S) || input_manager->KeyPressed(SDL_SCANCODE_DOWN))
    {
      selected_button = (selected_button + 1) % buttons_count;
    }
  }
  else if (current_scene == GameScenes::GAME_OVER)
  {
    if (input_manager->KeyPressed(SDL_SCANCODE_RETURN))
    {
      ChangeScene(GameScenes::MENU);
    }
  }

  ui_manager.RenderCurrentPanel();
}

void Game::InitEntityPools()
{
  for (int i = 0; i < entity_pools.size(); i++)
  {
    switch (EntityPoolType(i))
    {
    case EntityPoolType::PROJECTILE:
      entity_pools[i] = std::make_shared<ProjectilesPool>();
      entity_pools[i]->Init(150);
      break;
    case EntityPoolType::ENEMY:
      entity_pools[i] = std::make_shared<EnemiesPool>();
      entity_pools[i]->Init(100);
      break;
    case EntityPoolType::COLLECTABLE:
      entity_pools[i] = std::make_shared<CollectablesPool>();
      entity_pools[i]->Init(50);
      break;
    case EntityPoolType::EFFECTS:
      entity_pools[i] = std::make_shared<EffectsPool>();
      entity_pools[i]->Init(100);
      break;
    default:
      assert(false);
      break;
    }
  }
}

void Game::ResetGame()
{
  for (int i = 0; i < entity_pools.size(); i++)
  {
    GetPool(EntityPoolType(i))->Reset();
  }

  for (Entity entity : registry.view<entt::entity>())
  {
    DestroyEntity(entity);
  }
}

void Game::UpdateGame(float delta_time)
{
  if (!game_inited)
  {
    InitGame();
  }

  // Draw background
  SDL_RenderCopy(renderer, Game::GetResources().background_texture->texture.GetSDLTexture(), NULL, NULL);

  if (Game::GetRootEntity().HasComponent<GameStateSingleComponent>())
  {
    delta_time *= Game::GetRootEntity().GetComponent<GameStateSingleComponent>().time_scale;
  }

  systems_manager.Update(delta_time);
}

Entity Game::CreateEntity(const std::string& name)
{
  Entity entity(GetRegistry().create());
  auto& tag_comp = entity.AddComponent<TagComponent>();
  tag_comp.tag = name.empty() ? "Entity" : name;

  return entity;
}

void Game::UpdateScene(float delta_time)
{
  if (current_scene != next_scene)
  {
    // On scene changed
    switch (next_scene)
    {
    case GameScenes::MENU:
      game_inited = false;
      Game::GetResources().menu_music->Play();
      break;
    case GameScenes::GAME:
      Game::GetResources().game_music->Play();
      break;
    case GameScenes::GAME_OVER:
      Game::GetResources().game_over_music->Play(0);
      break;
    }

    current_scene = next_scene;
  }

  switch (current_scene)
  {
  case GameScenes::MENU:
    UpdateMenu(delta_time);
    break;
  case GameScenes::GAME:
    UpdateGame(delta_time);
    UpdateMenu(delta_time);
    break;
  case GameScenes::GAME_OVER:
    UpdateGame(delta_time);
    UpdateMenu(delta_time);
    break;
  }
}

void Game::Run()
{
  LOG_INFO("Run Game...");

  if (!running)
  {
    return;
  }

  // Assets loading
  resourses.LoadResources();

  // Create UI Panels
  InitGUI();

  InitEntityPools();

  // Register ecs systems
  systems_manager.Register<CollisionSystem>();
  systems_manager.Register<PlayerInputSystem>();
  systems_manager.Register<MoveSystem>();
  systems_manager.Register<EnemyMoveSystem>();
  systems_manager.Register<ShootSystem>();
  systems_manager.Register<ProjectileSystem>();
  systems_manager.Register<PhysicsSystem>();
  systems_manager.Register<CollectableSystem>();
  systems_manager.Register<DamageSystem>();
  systems_manager.Register<ThrusterSystem>();
  systems_manager.Register<DeathSystem>();
  systems_manager.Register<SpriteSystem>();
  systems_manager.Register<DebugDrawSystem>();
  systems_manager.Register<HUDSystem>();
  systems_manager.Register<GameStateSystem>();

  while (running)
  {
    std::chrono::high_resolution_clock::time_point time = std::chrono::high_resolution_clock::now();
    float delta_time = std::chrono::duration<float>(time - last_frame_time).count();
    last_frame_time = time;

    ProcessEvents();

    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);

    UpdateScene(delta_time);

    SDL_RenderPresent(renderer);
  }
}

void Game::Close()
{
  LOG_INFO("Close Game");

  running = false;

  ui_manager.Clear();
  AssetManager::Clear();

  SDL_DestroyRenderer(renderer);
  renderer = nullptr;

  //Quit SDL subsystems
  TTF_Quit();
  IMG_Quit();
  Mix_Quit();
  SDL_Quit();
}

void Game::DestroyEntity(Entity entity)
{
  if (Game::HasEntity(entity))
  {
    GetRegistry().destroy(entity);
  }
}

bool Game::HasEntity(Entity entity)
{
  return GetRegistry().valid(entity) && GetRegistry().all_of(entity);
}

Entity Game::GetRootEntity()
{
  return GetInstance()->root_entity;
}

ui::Panel& Game::GetHUD()
{
  return GetInstance()->ui_manager.GetPanel(uint32_t(GameScenes::GAME));
}

ui::UIManager& Game::GetUI()
{
  return GetInstance()->ui_manager;
}

void Game::ChangeScene(GameScenes scene)
{
  GetInstance()->next_scene = scene;
}

void Game::ProcessEvents()
{
  // Events polling
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
    case SDL_QUIT:
      running = false;
      break;
    case SDL_WINDOWEVENT:
      if (e.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        LOG_INFO("Resizing window {} {}", e.window.data1, e.window.data2);

        window.SetWidth(e.window.data1);
        window.SetHeight(e.window.data2);
      }
      break;

    default:
      break;
    }
  }

  // User input processing
  auto input_manager = tools::InputManager::Instance();
  input_manager->UpdateState();

  if (input_manager->KeyPressed(SDL_SCANCODE_ESCAPE))
  {
    if (current_scene == GameScenes::MENU)
    {
      running = false;
    }
    else
    {
      next_scene = GameScenes::MENU;
    }
  }
  else if (input_manager->KeyPressed(SDL_SCANCODE_F10))
  {
    Game::GetWindow().SetFullscreen(!Game::GetWindow().IsFullscreen());
  }
}