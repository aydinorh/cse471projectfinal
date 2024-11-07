using namespace std;
#pragma once
#include <vector>
#include <memory>

class CEffect {
public:
    // Constructor with sample rate for initializing effects
    explicit CEffect(float sampleRate);
    virtual ~CEffect() = default;

    // Set chorus parameters
    void SetChorusDepth(float depth);
    void SetChorusRate(float rate);
    void SetChorusWetDryMix(float mix);

    // Process a single audio sample through the effect
    virtual float ProcessSample(float input) = 0;

    // Set the wet/dry mix for the effect
    virtual void SetWetDryMix(float mix) { m_wetDryMix = mix; }

    // Enable or disable the effect
    void Enable(bool enable) { m_enabled = enable; }
    bool IsEnabled() const { return m_enabled; }

private:
    
    std::vector<std::unique_ptr<CEffect>> m_effects;  // List of effect pointers for easy processing

protected:
    float m_sampleRate;
    float m_wetDryMix = 0.5f;  // Default wet/dry mix (50% wet, 50% dry)
    bool m_enabled = false;
};