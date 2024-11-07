#include "stdafx.h"
#include "CFlangerEffect.h"
#include <cmath>
#include <algorithm>

#define M_PI 3.14159265358979323846

CFlangerEffect::CFlangerEffect(float sampleRate)
    : CEffect(sampleRate), m_depth(10.0f), m_rate(0.25f), m_feedback(0.8f), m_wetDryMix(0.5f), m_lfoPhase(0.0f), m_bufferIndex(0) {
    int maxDelaySamples = static_cast<int>(sampleRate * 0.02f);  // Max delay of 20 ms
    m_delayBuffer.resize(maxDelaySamples, 0.0f);
}

void CFlangerEffect::SetDepth(float depth) {
    m_depth = depth;
}

void CFlangerEffect::SetRate(float rate) {
    m_rate = rate;
}

void CFlangerEffect::SetFeedback(float feedback) {
    m_feedback = (feedback < -0.9f) ? -0.9f : (feedback > 0.9f ? 0.9f : feedback);
}

void CFlangerEffect::SetWetDryMix(float mix) {
    m_wetDryMix = (mix < 0.0f) ? 0.0f : (mix > 1.0f ? 1.0f : mix);
}

float CFlangerEffect::ProcessSample(float input) {
    if (!m_enabled) {
        return input;  // Bypass effect if disabled
    }

    // Choose LFO shape (sine wave by default)
    float lfoValue = std::sin(2.0f * M_PI * m_lfoPhase);
    float delayTimeMs = m_depth * (0.5f * lfoValue + 0.5f);  // Modulated delay time
    int delaySamples = static_cast<int>((delayTimeMs / 1000.0f) * m_sampleRate);  // Convert ms to samples

    // Increment the LFO phase for smooth oscillation
    m_lfoPhase += m_rate / m_sampleRate;
    if (m_lfoPhase >= 1.0f) m_lfoPhase -= 1.0f;

    // Calculate read position in circular delay buffer
    int readIndex = m_bufferIndex - delaySamples;
    if (readIndex < 0) readIndex += m_delayBuffer.size();

    // Retrieve delayed sample and apply feedback
    float delayedSample = m_delayBuffer[readIndex];
    float output = input + delayedSample * m_feedback;

    // Store the current sample in the delay buffer with feedback applied
    m_delayBuffer[m_bufferIndex] = input + (delayedSample * m_feedback);
    m_bufferIndex = (m_bufferIndex + 1) % m_delayBuffer.size();

    // Mix the wet and dry signals
    return ((1.0f - m_wetDryMix) * input) + (m_wetDryMix * output);
}