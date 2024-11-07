#include "stdafx.h"
#include "SineWave.h"
#include <cmath>

CSineWave::CSineWave()
{
	m_phase = 0;
	m_amp = 0.1;
	m_freq = 440;
	m_threshold = 0.0;
    m_compressionThreshold = 0.0;
    m_compressionRatio = 1.0;
}


CSineWave::~CSineWave()
{
}

void CSineWave::Start()
{
	m_phase = 0;
}

void CSineWave::SetThreshold(float threshold) 
{
	m_threshold = threshold;
}


bool CSineWave::Generate()
{
    // Calculate the sample frame as before
    m_frame[0] = m_amp * sin(m_phase * 2 * PI);
    m_frame[1] = m_frame[0];

    // Apply the noise gate: if the absolute amplitude is below the threshold, silence the sample
    if (std::abs(m_frame[0]) < m_threshold) {
        m_frame[0] = 0.0f;
        m_frame[1] = 0.0f;
    }

    // Apply compression
    for (int i = 0; i < 2; i++)
    {
        if (std::abs(m_frame[i]) > m_compressionThreshold)
        {
            float sign = m_frame[i] > 0 ? 1 : -1;
            m_frame[i] = sign * (m_compressionThreshold + (std::abs(m_frame[i]) - m_compressionThreshold) / m_compressionRatio);
        }
    }

    // Increment the phase as before
    m_phase += m_freq * GetSamplePeriod();

    return true;
}