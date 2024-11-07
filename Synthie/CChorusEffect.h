#pragma once
#include "AudioNode.h"
#include <vector>

class CChorusEffect : public CAudioNode
{
public:
    CChorusEffect();
    virtual ~CChorusEffect();

    virtual void Start() override;
    virtual bool Generate() override;

    void SetDelayTime(double delayTime) { m_delayTime = delayTime; }
    void SetDepth(double depth) { m_depth = depth; }
    void SetRate(double rate) { m_rate = rate; }
    void Process(double* frame, int numChannels);
    void SetParameters(double delayMs, double depth, double rate);

private:
    double m_delayTime;         // Base delay time in seconds
    double m_depth;             // Depth of modulation (in seconds)
    double m_rate;              // Rate of modulation (Hz)
    std::vector<double> m_delayBuffer;
    int m_writeIndex;
    int m_bufferSize;
    double m_phase;
    int m_delayBufferPos;
    double m_delayMs;           // Delay time in milliseconds
};