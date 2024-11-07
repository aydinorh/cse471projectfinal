#include "stdafx.h"
#include "ChorusEffect.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

CChorusEffect::CChorusEffect(float sampleRate)
    : CEffect(sampleRate),  // Call the base Effect constructor with sampleRate
    m_delayBuffer(44100), m_delayIndex(0),
    m_depth(5.0f), m_rate(0.25f), m_lfoPhase(0.0f) {
}

// Set the modulation depth in milliseconds
void CChorusEffect::SetDepth(float depth) {
    m_depth = depth;
}

// Set the LFO rate in Hertz
void CChorusEffect::SetRate(float rate) {
    m_rate = rate;
}

// Process a single sample with the chorus effect
float CChorusEffect::ProcessSample(float input) 
{

    if (!m_enabled) {
        return input;  // Bypass effect if disabled
    }

    // LFO-modulated delay time
    float delayTime = 10.0f + m_depth * std::sin(2.0f * M_PI * m_lfoPhase);
    m_lfoPhase += m_rate / m_sampleRate;
    if (m_lfoPhase >= 1.0f) m_lfoPhase -= 1.0f;

    // Convert delay time to samples
    float delaySamples = (delayTime / 1000.0f) * m_sampleRate;
    float readIndex = m_delayIndex - delaySamples;
    if (readIndex < 0) readIndex += m_delayBuffer.size();

    // Linear interpolation for smoother delay access
    int index1 = static_cast<int>(readIndex);
    int index2 = (index1 + 1) % m_delayBuffer.size();
    float frac = readIndex - index1;
    float delayedSample = (1.0f - frac) * m_delayBuffer[index1] + frac * m_delayBuffer[index2];

    // Update delay buffer
    m_delayBuffer[m_delayIndex] = input;
    m_delayIndex = (m_delayIndex + 1) % m_delayBuffer.size();

    // Mix dry and wet signals
    return ((1.0f - m_wetDryMix) * input) + (m_wetDryMix * delayedSample);
}