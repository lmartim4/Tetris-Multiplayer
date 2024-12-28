#include <iostream>
#include <chrono>
#include <string>

class Clock
{
public:
    // Constructor that allows you to name the measurement.
    // e.g., Clock("Loading Assets"), Clock("Physics Simulation"), etc.
    explicit Clock(const std::string &measurementName = "Unnamed Measurement")
        : measurementName_(measurementName)
    {
    }

    // Start recording time
    void start()
    {
        startTime_ = std::chrono::high_resolution_clock::now();
    }

    // Stop recording time, print duration in microseconds
    void end()
    {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto durationMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(
                                        endTime - startTime_)
                                        .count();
        std::cout << "Elapsed time [" << measurementName_ << "]: "
                  << durationMicroseconds << " microseconds\n";
    }

    // Get raw duration in microseconds without printing
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