#include <cmath>
#include "AllPassFilters.h"

//==============================================================================
FirstOrderAllPass::FirstOrderAllPass()
{
}

void FirstOrderAllPass::init(int sampleRate)
{
	m_SampleRate = sampleRate;
}

void FirstOrderAllPass::setFrequency(float frequency)
{
	if (m_SampleRate == 0)
	{
		return;
	}

	const float tmp = tanf(PI * frequency / m_SampleRate);
	m_a1 = (tmp - 1.0f) / (tmp + 1.0f);
}


float FirstOrderAllPass::process(float in)
{
	const float tmp = m_a1 * in + m_d;
	m_d = in - m_a1 * tmp;
	return tmp;
}

//==============================================================================
SecondOrderAllPass::SecondOrderAllPass()
{
}

void SecondOrderAllPass::init(int sampleRate)
{
	m_SampleRate = sampleRate;
}

void SecondOrderAllPass::setFrequency(float frequency, float Q)
{
	if (m_SampleRate == 0)
	{
		return;
	}

	const float w = 2.0f * PI * frequency / m_SampleRate;
	const float cosw = cos(w);
	const float alpha = sin(w) * (2.0f * Q);

	const float a2 = 1 + alpha;

	m_a0 = (1.0f - alpha) / a2;
	m_a1 = (-2.0f * cosw) / a2;
}

float SecondOrderAllPass::process(float in)
{
	const float yn = m_a0 * (in - m_ynz2) + m_a1 * (m_xnz1 - m_ynz1) + m_xnz2;
	
	m_xnz2 = m_xnz1;
	m_xnz1 = in;
	m_ynz2 = m_ynz1;
	m_ynz1 = yn;
	
	return yn;
}