#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace tools
{
class Logger
{
public:
  static const char* default_log_pattern;

  static void Init(spdlog::level::level_enum log_level, const std::string& pattern = default_log_pattern);
  static std::shared_ptr<spdlog::logger> get_logger()
  {
    return logger;
  };

private:
  static std::shared_ptr<spdlog::logger> logger;
  static std::vector<spdlog::sink_ptr> sinks;
};
} // namespace tools

#define LOG_TRACE(...) ::tools::Logger::get_logger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::tools::Logger::get_logger()->debug(__VA_ARGS__)
#define LOG_INFO(...) ::tools::Logger::get_logger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::tools::Logger::get_logger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::tools::Logger::get_logger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::tools::Logger::get_logger()->critical(__VA_ARGS__)
#define LOG_FATAL(...) ::tools::Logger::get_logger()->fatal(__VA_ARGS__)
