using namespace std;
#pragma once
#include "Effect.h"
#include <vector>
#include <cmath>  // For sin and M_PI

class CChorusEffect : public CEffect {
public:
    explicit CChorusEffect(float sampleRate);

    // Set modulation depth (in milliseconds)
    void SetDepth(float depth);

    // Set LFO rate (in Hertz)
    void SetRate(float rate);

    // Override ProcessSample from the base class
    float ProcessSample(float input) override;

private:
    //float m_sampleRate;
    std::vector<float> m_delayBuffer;
    int m_delayIndex;

    float m_depth;       // Modulation depth in ms
    float m_rate;        // LFO rate in Hz
    float m_lfoPhase;    // Current phase of the LFO
};