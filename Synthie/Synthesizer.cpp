#include "stdafx.h"
#include "Synthesizer.h"
#include "Instrument.h"
#include "ToneInstrument.h"
#include "xmlhelp.h"
#include <vector>
#include <algorithm>

CSynthesizer::CSynthesizer()
{
	CoInitialize(NULL);
	m_channels = 2;
	m_sampleRate = 44100.0;
	m_samplePeriod = 1.0 / m_sampleRate;
	m_bpm = 120;            
	m_beatspermeasure = 4;
	m_secperbeat = 60.0/m_bpm;   
	m_effect = std::make_unique<CChorusEffect>(m_sampleRate);
	m_reverbEffect = std::make_unique<CReverbEffect>(m_sampleRate);
	m_flangerEffect = std::make_unique<CFlangerEffect>(m_sampleRate);
}


void CSynthesizer::SetChorusDepth(float depth) {
	m_effect->SetChorusDepth(depth);
}


void CSynthesizer::SetChorusRate(float rate) {
	m_effect->SetChorusRate(rate);
}


void CSynthesizer::SetChorusWetDryMix(float mix) {
	m_effect->SetWetDryMix(mix);
}


void CSynthesizer::EnableChorus(bool enable) {
	m_effect->Enable(enable);
}

void CSynthesizer::EnableReverb(bool enable_rev) {
	m_reverbEffect->Enable(enable_rev);
}

void CSynthesizer::EnableFlanger(bool enable_flan) {
	m_flangerEffect->Enable(enable_flan);
}


CSynthesizer::~CSynthesizer()
{
}

//! Start the synthesizer
void CSynthesizer::Start()
{
	/*m_time = 0;
	CToneInstrument *ti = new CToneInstrument();
	ti->SetSampleRate(GetSampleRate());
	ti->SetFreq(440);
	ti->SetDuration(3);
	ti->Start();
	m_instruments.push_back(ti);*/
	m_instruments.clear();
	m_currentNote = 0;
	m_measure = 0;
	m_beat = 0;
	m_time = 0;

	//m_chorusEffect.SetDelayTime(0.03);  
	///m_chorusEffect.SetDepth(0.002);     
	//m_chorusEffect.SetRate(1.0);
}


bool CSynthesizer::Generate(double * frame)
{
	/*double sample = 0.1 * sin(2 * PI * 440 * GetTime());

	for (int c = 0; c<GetNumChannels(); c++)
	{
		frame[c] = sample;
	}

	m_time += GetSamplePeriod();
	return m_time < 5;*/

	//
	// Phase 1: Determine if any notes need to be played.
	//

	while (m_currentNote < (int)m_notes.size())
	{
		// Get a pointer to the current note
		CNote *note = &m_notes[m_currentNote];

		// If the measure is in the future we can't play
		// this note just yet.
		if (note->Measure() > m_measure)
			break;

		// If this is the current measure, but the
		// beat has not been reached, we can't play
		// this note.
		if (note->Measure() == m_measure && note->Beat() > m_beat)
			break;

		//
		// Play the note!
		//

		// Create the instrument object
		CInstrument *instrument = NULL;
		if (note->Instrument() == L"ToneInstrument")
		{
			instrument = new CToneInstrument(GetBeatsPerMinute());
		}

		if (note->Instrument() == L"CDrumInstrument")
		{
			auto drum = new CDrumInstrument();
			drum->SetSampleRate(GetSampleRate());
			drum->SetNote(note);  // This will set the drum type based on the note information
			drum->Start();
			instrument = drum;
		}

		// Configure the instrument object
		if (instrument != NULL)
		{
			instrument->SetSampleRate(GetSampleRate());
			instrument->SetNote(note);
			instrument->Start();

			m_instruments.push_back(instrument);
		}

		m_currentNote++;
	}
	//
	// Phase 2: Clear all channels to silence 
	//

	for (int c = 0; c<GetNumChannels(); c++)
	{
		frame[c] = 0;
	}

	//
	// Phase 3: Play an active instruments
	//

	//
	// We have a list of active (playing) instruments.  We iterate over 
	// that list.  For each instrument we call generate, then add the
	// output to our output frame.  If an instrument is done (Generate()
	// returns false), we remove it from the list.
	//

	for (list<CInstrument *>::iterator node = m_instruments.begin(); node != m_instruments.end();)
	{
		// Since we may be removing an item from the list, we need to know in 
		// advance, what is after it in the list.  We keep that node as "next"
		list<CInstrument *>::iterator next = node;
		next++;

		// Get a pointer to the allocated instrument
		CInstrument *instrument = *node;

		// Call the generate function
		if (instrument->Generate())
		{
			// If we returned true, we have a valid sample.  Add it 
			// to the frame.
			for (int c = 0; c<GetNumChannels(); c++)
			{
				float sample = static_cast<float>(instrument->Frame(c));

				if (m_flangerEffect && m_flangerEffect->IsEnabled()) {
					frame[c] += m_flangerEffect->ProcessSample(sample);
				}

				else if (m_reverbEffect && m_reverbEffect->IsEnabled()) {
					frame[c] += m_reverbEffect->ProcessSample(sample);
				}

				else if (m_effect && m_effect->IsEnabled()) {
					frame[c] += m_effect->ProcessSample(sample);
				}
				else {
					// If m_effect is null or disabled, add instrument's output directly
					frame[c] += instrument->Frame(c);
				}
			}
		}
		else
		{
			// If we returned false, the instrument is done.  Remove it
			// from the list and delete it from memory.
			m_instruments.erase(node);
			delete instrument;
		}

		// Move to the next instrument in the list
		node = next;
	}
	//
	// Phase 4: Advance the time and beats
	//

	// Time advances by the sample period
	m_time += GetSamplePeriod();

	// Beat advances by the sample period divided by the 
	// number of seconds per beat.  The inverse of seconds
	// per beat is beats per second.
	m_beat += GetSamplePeriod() / m_secperbeat;

	// When the measure is complete, we move to
	// a new measure.  We might be a fraction into
	// the new measure, so we subtract out rather 
	// than just setting to zero.
	if (m_beat > m_beatspermeasure)
	{
		m_beat -= m_beatspermeasure;
		m_measure++;
	}
	//
	// Phase 5: Determine when we are done
	//

	// We are done when there is nothing to play.  We'll put something more 
	// complex here later.
	return !m_instruments.empty() || m_currentNote < (int)m_notes.size();
}


// Get the time since we started generating audio
double CSynthesizer::GetTime()
{
	return m_time;
}
void CSynthesizer::Clear()
{
	m_instruments.clear();
	m_notes.clear();
}
void CSynthesizer::XmlLoadScore(IXMLDOMNode * xml)
{
	// Get a list of all attribute nodes and the
	// length of that list

	EnableReverb(false);
	EnableChorus(false);
	EnableFlanger(false);
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i<len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		// Get the value of the attribute.  A CComVariant is a variable
		// that can have any type. It loads the attribute value as a
		// string (UNICODE), but we can then change it to an integer 
		// (VT_I4) or double (VT_R8) using the ChangeType function 
		// and then read its integer or double value from a member variable.
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == L"bpm")
		{
			value.ChangeType(VT_R8);
			m_bpm = value.dblVal;
			m_secperbeat = 1 / (m_bpm / 60);
		}
		else if (name == L"beatspermeasure")
		{
			value.ChangeType(VT_I4);
			m_beatspermeasure = value.intVal;
		}

	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);
		if (name == L"instrument")
		{
			XmlLoadInstrument(node);
		}
	}
}
void CSynthesizer::OpenScore(CString & filename)
{
	Clear();

	//
	// Create an XML document
	//

	CComPtr<IXMLDOMDocument>  pXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,
		IID_IXMLDOMDocument, (void**)&pXMLDoc));
	if (!succeeded)
	{
		AfxMessageBox(L"Failed to create an XML document to use");
		return;
	}

	// Open the XML document
	VARIANT_BOOL ok;
	succeeded = SUCCEEDED(pXMLDoc->load(CComVariant(filename), &ok));
	if (!succeeded || ok == VARIANT_FALSE)
	{
		AfxMessageBox(L"Failed to open XML score file");
		return;
	}

	//
	// Traverse the XML document in memory!!!!
	// Top level tag is <score>
	//

	CComPtr<IXMLDOMNode> node;
	pXMLDoc->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR nodeName;
		node->get_nodeName(&nodeName);
		if (nodeName == "score")
		{
			XmlLoadScore(node);
		}
	}
	sort(m_notes.begin(), m_notes.end());
}
void CSynthesizer::XmlLoadInstrument(IXMLDOMNode * xml)
{
	wstring instrument = L"";

	// Get a list of all attribute nodes and the
	// length of that list
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	// Loop over the list of attributes
	for (int i = 0; i<len; i++)
	{
		// Get attribute i
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		// Get the name of the attribute
		CComBSTR name;
		attrib->get_nodeName(&name);

		// Get the value of the attribute.  
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == "instrument")
		{
			instrument = value.bstrVal;
		}
	}


	CComPtr<IXMLDOMNode> node;
	xml->get_firstChild(&node);
	for (; node != NULL; NextNode(node))
	{
		// Get the name of the node
		CComBSTR name;
		node->get_nodeName(&name);

		if (name == L"note")
		{
			XmlLoadNote(node, instrument);
		}
	}

}
void CSynthesizer::XmlLoadNote(IXMLDOMNode * xml, std::wstring & instrument)
{
	m_notes.push_back(CNote());
	m_notes.back().XmlLoad(xml, instrument);
	CComPtr<IXMLDOMNamedNodeMap> attributes;
	xml->get_attributes(&attributes);
	long len;
	attributes->get_length(&len);

	for (int i = 0; i < len; i++) {
		CComPtr<IXMLDOMNode> attrib;
		attributes->get_item(i, &attrib);

		CComBSTR name;
		attrib->get_nodeName(&name);
		CComVariant value;
		attrib->get_nodeValue(&value);

		if (name == L"chorus_enable") {
			value.ChangeType(VT_BSTR);
			std::wstring enableValue = value.bstrVal;

			// Check if the value is "true" or "false"
			if (enableValue == L"true") {
				EnableChorus(true);
			}
			else if (enableValue == L"false") {
				EnableChorus(false);
			}
		}

		else if (name == L"reverb_enable")
		{
			value.ChangeType(VT_BSTR);
			std::wstring enableValue = value.bstrVal;

			if (enableValue == L"true") {
				EnableReverb(true);  // Assuming EnableReverb is defined
			}
			else if (enableValue == L"False") {
				EnableReverb(false);
			}
		}

		else if (name == L"flanger_enable")
		{
			value.ChangeType(VT_BSTR);
			std::wstring enableValue = value.bstrVal;

			if (enableValue == L"true") {
				EnableFlanger(true);  // Assuming EnableReverb is defined
				m_flangerEffect->SetDepth(7.0f);       // 7 ms modulation depth
				m_flangerEffect->SetRate(0.2f);        // 0.2 Hz modulation rate
				m_flangerEffect->SetFeedback(0.8f);    // High feedback for stronger effect
				m_flangerEffect->SetWetDryMix(0.5f);   // 50% wet/dry mix
			}
			else if (enableValue == L"false") {
				EnableFlanger(false);
			}
		}
	}
}