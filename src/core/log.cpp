#include "core/log.h"

#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace tools
{
const char* Logger::default_log_pattern = "%^%T <%n> [%l] [%t] %v%$";

std::shared_ptr<spdlog::logger> Logger::logger;
std::vector<spdlog::sink_ptr> Logger::sinks;

void Logger::Init(spdlog::level::level_enum log_level, const std::string& pattern)
{
  sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log/log.txt", 2 * 1024 * 1024, 3));
  sinks.push_back(std::make_shared<spdlog::sinks::msvc_sink_mt>("GAME"));

  logger = std::make_shared<spdlog::logger>("GAME", std::begin(sinks), std::end(sinks));
  logger->flush_on(log_level);
  logger->set_level(log_level);
  logger->set_pattern(pattern);
}
} // namespace tools
