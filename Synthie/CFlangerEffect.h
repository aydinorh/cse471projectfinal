#pragma once
#include "Effect.h"
class CFlangerEffect : public CEffect
{
public:
    explicit CFlangerEffect(float sampleRate);

    void SetDepth(float depth);         // Depth of modulation (maximum delay time in milliseconds)
    void SetRate(float rate);           // Rate of modulation (frequency of LFO in Hz)
    void SetFeedback(float feedback);   // Controls feedback amount

    void SetWetDryMix(float mix) override;

    float ProcessSample(float input) override;

private:
    std::vector<float> m_delayBuffer;   // Buffer to store delayed samples
    float m_lfoPhase;                   // Current phase of the LFO
    float m_depth;                      // Maximum delay time in milliseconds
    float m_rate;                       // Frequency of the LFO in Hz
    float m_feedback;   
    float m_wetDryMix;
    int m_bufferIndex;                  // Current write position in the delay buffer
};

