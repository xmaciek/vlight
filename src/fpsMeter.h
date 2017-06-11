#pragma once

#include "includeShortcut.h"
#include "macros.hpp"
#include <chrono>
#include <cstdint>

class fpsMeter {
    DISABLE_COPY( fpsMeter )

private:
    typedef std::chrono::system_clock::duration Duration;
    typedef std::chrono::system_clock::time_point TimePoint;
    Duration m_totalDuration;
    std::uint64_t m_fps;

    Duration m_framesDuration;
    TimePoint m_currentFrameBegin;
    TimePoint m_lastUpdate;
    std::uint64_t m_framesDone;

public:
    fpsMeter();
    void beginFrame();
    void endFrame();
    void clear();
    void update();
    std::uint64_t fps() const;
    std::uint64_t calcFps() const;
    double averageFrame() const;
};
