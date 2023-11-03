#include "Logging/Logging.h"

std::string Frostbyte::Logger::GetTimeAsString()
{
    time_t time = std::time(nullptr);
    auto localTime = *std::localtime(&time);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "[%H:%M:%S]");
    return oss.str();
}
