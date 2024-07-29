#include "core/log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace tools
{
  const char* Logger::default_log_pattern = "%^%T <%n> [%l] [%t] %v%$";

  std::shared_ptr<spdlog::logger> Logger::_logger;

  void Logger::Init(spdlog::level::level_enum log_level, const std::string& pattern)
  {
    _logger = spdlog::stdout_color_mt("GAME");

    spdlog::set_level(log_level);
    spdlog::set_pattern(pattern);

    _logger->set_level(log_level);
  }
} // namespace tools
