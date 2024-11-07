#include "stdafx.h"
#include "CReverbEffect.h"
#include <cmath>

CReverbEffect::CReverbEffect(float sampleRate)
    : CEffect(sampleRate), m_decay(0.5f), m_wetDryMix(0.5f), m_bufferIndex(0) {
    int maxDelaySamples = static_cast<int>(sampleRate * 0.5f);  // Maximum delay buffer of 0.5 seconds
    m_delayBuffer.resize(maxDelaySamples, 0.0f);

    // Set up multiple delay taps to create reflections (in samples)
    m_tapDelays = { 500, 1000, 1500, 2500, 3200, 4200, 5100, 6200 };  // Adjust these values for different spaces
}

void CReverbEffect::SetDecay(float decay) {
    m_decay = decay;
}

void CReverbEffect::SetMix(float mix) {
    m_wetDryMix = mix;
}

float CReverbEffect::ProcessSample(float input) {
    if (!m_enabled) {
        return input;  // Bypass effect if disabled
    }

    // Sum up delayed samples from each tap
    float reverbOutput = 0.0f;
    for (int tap : m_tapDelays) {
        int tapIndex = (m_bufferIndex - tap + m_delayBuffer.size()) % m_delayBuffer.size();
        reverbOutput += m_delayBuffer[tapIndex] * m_decay;
    }

    // Mix the input signal and reverb output
    float output = ((1.0f - m_wetDryMix) * input) + (m_wetDryMix * reverbOutput);

    // Store the current input plus a portion of the reverb output in the delay buffer
    m_delayBuffer[m_bufferIndex] = input + (reverbOutput * m_decay * 0.8f);  // Adding feedback for smoother decay
    m_bufferIndex = (m_bufferIndex + 1) % m_delayBuffer.size();

    return output;
}