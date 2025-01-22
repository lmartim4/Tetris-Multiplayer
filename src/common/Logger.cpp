#include "Logger.hpp"

void Logger::console_log(const std::string &msg) const
{

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&in_time_t);

    std::cout
        << std::put_time(&tm, "[%H:%M:%S]")
        << " [" << className << "] "
        << " " << msg << std::endl;
}

Logger::Logger(std::string name) : className(name)
{
}

Logger::~Logger()
{
}
