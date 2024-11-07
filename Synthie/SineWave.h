#pragma once
#include "AudioNode.h"
class CSineWave :
	public CAudioNode
{
public:
	CSineWave();
	virtual ~CSineWave();
public:
	//! Start audio generation
	virtual void Start();

	//! Generate one frame of audio
	virtual bool Generate();

	//! Set the sine wave frequency
	void SetFreq(double f) { m_freq = f; }

	//! Set the sine wave amplitude
	void SetAmplitude(double a) { m_amp = a; }

	void SetThreshold(float threshold); // set noise gate threshold

	void SetCompressionThreshold(float threshold) { m_compressionThreshold = threshold; }
	void SetCompressionRatio(float ratio) { m_compressionRatio = ratio; }

private:
	double m_freq;
	double m_amp;
	double m_phase;
	float m_threshold; 
	float m_compressionThreshold;
	float m_compressionRatio;
};

