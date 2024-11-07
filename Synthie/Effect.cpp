#include "stdafx.h"
#include "Effect.h"
#include "ChorusEffect.h"

// Constructor to initialize effects with sample rate
CEffect::CEffect(float sampleRate)
    : m_sampleRate(sampleRate) {
   
}

// Set chorus effect parameters
void CEffect::SetChorusDepth(float depth) {
    if (auto* chorus = dynamic_cast<CChorusEffect*>(m_effects[0].get())) {
        chorus->SetDepth(depth);
    }
}

void CEffect::SetChorusRate(float rate) {
    if (auto* chorus = dynamic_cast<CChorusEffect*>(m_effects[0].get())) {
        chorus->SetRate(rate);
    }
}

void CEffect::SetChorusWetDryMix(float mix) {
    for (auto& effect : m_effects) {
        effect->SetWetDryMix(mix);
    }
}

// Process a single sample through all active effects
float CEffect::ProcessSample(float input) {
    float processedSample = input;

    // Process each effect in the list
    for (auto& effect : m_effects) {
        processedSample = effect->ProcessSample(processedSample);  // Chain each effect
    }
    return processedSample;
}