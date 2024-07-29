#include "game/game.h"
#include "core/log.h"

#define RYML_SINGLE_HDR_DEFINE_NOW

int main(int argc, char* args[])
{
  GameParams app_params;
  app_params.window_name = "SpaceWar!";
  app_params.screen_width = 1600;
  app_params.screen_height = 900;

  tools::Logger::Init(app_params.log_level);

  Game game(app_params);
  game.Run();

  return 0;
}
