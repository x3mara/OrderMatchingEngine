#pragma once
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

inline std::string formatTimestamp(std::chrono::system_clock::time_point tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tmStruct{};

#ifdef _WIN32
    localtime_s(&tmStruct, &time);   // MSVC-safe version, since you're on Visual Studio
#else
    localtime_r(&time, &tmStruct);   // POSIX-safe version
#endif

    std::ostringstream oss;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
    tp.time_since_epoch()) % 1000;
    oss << std::put_time(&tmStruct, "%Y-%m-%d %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}