#include "stdafx.h"
#include "CDrumInstrument.h"
#include <cmath>
#include <cstdlib>


CDrumInstrument::CDrumInstrument() : m_duration(0.5), m_time(0)
{
}

CDrumInstrument::~CDrumInstrument()
{
}

void CDrumInstrument::Start()
{
    m_time = 0;

    // Set sample rate if needed and initialize any ADSR or envelope settings here.
}

bool CDrumInstrument::Generate()
{
    if (m_drumType == L"bass")
        GenerateBassDrum();
    else if (m_drumType == L"snare")
        GenerateSnare();
    else if (m_drumType == L"tom")
        GenerateTom();
    else if (m_drumType == L"cymbal")
        GenerateCymbal();

    // Advance time and check if we've reached the end of the drum hit
    m_time += GetSamplePeriod();
    return m_time < m_duration;
}

void CDrumInstrument::SetNote(CNote* note)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    note->Node()->get_attributes(&attributes);
    long len;
    attributes->get_length(&len);

    for (int i = 0; i < len; i++)
    {
        CComPtr<IXMLDOMNode> attrib;
        attributes->get_item(i, &attrib);

        CComBSTR name;
        attrib->get_nodeName(&name);

        CComVariant value;
        attrib->get_nodeValue(&value);

        if (name == L"type")
        {
            SetDrumType(value.bstrVal);
        }
    }
}

void CDrumInstrument::SetDrumType(const std::wstring& type)
{
    m_drumType = type;
}

// Placeholder methods for drum sounds
void CDrumInstrument::GenerateBassDrum()
{
    double frequency = 60 * (1.0 - m_time / m_duration);  // Lower base frequency
    double amplitude = exp(-m_time * 5);  // Fast decay
    m_frame[0] = amplitude * sin(2 * PI * frequency * m_time);
    m_frame[1] = m_frame[0];
}

void CDrumInstrument::GenerateSnare()
{
    double amplitude = exp(-m_time * 15);  // Fast decay
    m_frame[0] = amplitude * (((double)rand() / RAND_MAX) * 2.0 - 1.0);
    m_frame[1] = m_frame[0];
}

void CDrumInstrument::GenerateTom()
{
    double frequency = 150 * (1.0 - m_time / m_duration);  // Mid-range frequency
    double amplitude = exp(-m_time * 7);  // Moderate decay
    m_frame[0] = amplitude * sin(2 * PI * frequency * m_time);
    m_frame[1] = m_frame[0];
}

void CDrumInstrument::GenerateCymbal()
{
    double amplitude = exp(-m_time * 10);  // Longer decay for sustain
    m_frame[0] = amplitude * (((double)rand() / RAND_MAX) * 2.0 - 1.0);
    m_frame[1] = m_frame[0];
}