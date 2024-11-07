#include "stdafx.h"
#include "CChorusEffect.h"
#include <cmath>

CChorusEffect::CChorusEffect()
    : m_delayTime(0.02), m_depth(0.002), m_rate(1.5), m_phase(0), m_writeIndex(0), m_delayBufferPos(0)
{
    m_bufferSize = static_cast<int>(GetSampleRate() * 0.05); // 50ms buffer
    m_delayBuffer.resize(m_bufferSize, 0.0);
}

CChorusEffect::~CChorusEffect() {}

void CChorusEffect::Start()
{
    std::fill(m_delayBuffer.begin(), m_delayBuffer.end(), 0.0);
    m_writeIndex = 0;
    m_phase = 0;
    m_delayBufferPos = 0;
}

bool CChorusEffect::Generate()
{
    int readIndex = m_writeIndex - static_cast<int>((m_delayTime + m_depth * sin(2 * PI * m_rate * m_phase)) * GetSampleRate());
    if (readIndex < 0)
        readIndex += m_bufferSize;

    m_frame[0] = m_delayBuffer[readIndex];
    m_frame[1] = m_frame[0];

    m_delayBuffer[m_writeIndex] = m_frame[0];

    m_writeIndex = (m_writeIndex + 1) % m_bufferSize;
    m_phase += GetSamplePeriod();

    return true;
}

void CChorusEffect::Process(double* frame, int numChannels)
{
    double modulatedDelay = m_delayTime + m_depth * sin(2 * PI * m_rate * m_phase);

    for (int c = 0; c < numChannels; c++) {
        int delaySamples = static_cast<int>(modulatedDelay * 44100);
        int readPos = (m_delayBufferPos - delaySamples + m_delayBuffer.size()) % m_delayBuffer.size();

        frame[c] += 0.5 * m_delayBuffer[readPos];

        m_delayBuffer[m_delayBufferPos] = frame[c];
    }

    m_delayBufferPos = (m_delayBufferPos + 1) % m_delayBuffer.size();
    m_phase += GetSamplePeriod();
}

void CChorusEffect::SetParameters(double delayMs, double depth, double rate)
{
    m_delayMs = delayMs;
    m_depth = depth;
    m_rate = rate;
}