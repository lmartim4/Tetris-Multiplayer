#pragma once

#include <iostream>
#include <chrono>
#include <string>

class Clock
{
public:
    explicit Clock(const std::string &measurementName = "Unnamed Measurement")
        : measurementName_(measurementName)
    {
    }

    void start()
    {
        startTime_ = std::chrono::high_resolution_clock::now();
    }

    void end()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                                        endTime - startTime_)
                                        .count();
        std::cout << "Elapsed time [" << measurementName_ << "]: "
                  << durationMicroseconds << " microseconds\n";
    }

    long long elapsedMicroseconds() const
    {
        auto currentTime = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   currentTime - startTime_)
            .count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime_;
    std::string measurementName_;
};