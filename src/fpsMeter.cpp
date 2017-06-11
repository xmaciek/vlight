#include "fpsMeter.h"

fpsMeter::fpsMeter()
: m_fps( 0 )
, m_framesDone( 0 )
{
}

void fpsMeter::beginFrame()
{
    m_currentFrameBegin = std::chrono::system_clock::now();
}


void fpsMeter::endFrame()
{
    m_framesDuration += std::chrono::system_clock::now() - m_currentFrameBegin;;
    m_framesDone++;
}

double fpsMeter::averageFrame() const
{
    return (double)std::chrono::duration_cast<std::chrono::milliseconds>( m_totalDuration ).count() / ( m_fps ? m_fps : 1 );
}

std::uint64_t fpsMeter::fps() const
{
    return m_fps;
}

std::uint64_t fpsMeter::calcFps() const
{
    return 1000.0 / averageFrame();
}

void fpsMeter::update()
{
    const fpsMeter::TimePoint now = std::chrono::system_clock::now();
    if  ( !std::chrono::duration_cast<std::chrono::seconds>( now - m_lastUpdate ).count() ) {
        return;
    }
    m_lastUpdate = now;
    m_fps = m_framesDone;
    m_framesDone = 0;
    m_totalDuration = m_framesDuration;
    m_framesDuration = fpsMeter::Duration();
}

