#pragma once
#include "Instrument.h"
#include <string>


class CDrumInstrument :
    public CInstrument
{

public:
    CDrumInstrument();
    virtual ~CDrumInstrument();

    virtual void Start() override;
    virtual bool Generate() override;
    virtual void SetNote(CNote* note) override;

    // Set type of drum sound
    void SetDrumType(const std::wstring& type);

private:
    // Drum properties
    std::wstring m_drumType;  // "bass", "snare", "tom", "cymbal"
    double m_duration;
    double m_time;

    // Drum-specific helper functions
    void GenerateBassDrum();
    void GenerateSnare();
    void GenerateTom();
    void GenerateCymbal();

};

