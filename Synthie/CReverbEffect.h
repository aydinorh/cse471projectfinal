#pragma once
#include "Effect.h"
#include <vector>

class CReverbEffect : public CEffect {
public:
    explicit CReverbEffect(float sampleRate);

    void SetDecay(float decay);         // Controls the decay time of reverb
    void SetMix(float mix);             // Controls the wet/dry mix

    float ProcessSample(float input) override;

private:
    std::vector<float> m_delayBuffer;   // Buffer to store delayed samples
    std::vector<int> m_tapDelays;       // Delay times for each reflection tap
    float m_decay;
    float m_wetDryMix;
    int m_bufferIndex;                  // Current write position in the delay buffer
};