#pragma once


#include <math.h>
//#include <algorithm>
//#include <mmintrin.h>

class Clippers
{

public:
	/*inline static float minss(float a, float b)
	{
		// Branchless SSE min.
		_mm_store_ss(&a, _mm_min_ss(_mm_set_ss(a), _mm_set_ss(b)));
		return a;
	}

	inline static float maxss(float a, float b)
	{
		// Branchless SSE max.
		_mm_store_ss(&a, _mm_max_ss(_mm_set_ss(a), _mm_set_ss(b)));
		return a;
	}

	inline static float clamp(float val, float minval, float maxval)
	{
		// Branchless SSE clamp.
		// return minss( maxss(val,minval), maxval );

		_mm_store_ss(&val, _mm_min_ss(_mm_max_ss(_mm_set_ss(val), _mm_set_ss(minval)), _mm_set_ss(maxval)));
		return val;
	}*/

	inline static float clamp(float val, float minval, float maxval)
	{
		const float t = val < minval ? minval : val;
		return t > maxval ? maxval : t;
	}

	inline static float HardClip(const float in, const float threshold)
	{
		/*if (in > threshold)
		{
			return threshold;
		}
		else if (in < -threshold)
		{
			return -threshold;
		}
		else
		{
			return in;
		}*/
		
		//return std::clamp(in, -threshold, threshold);
		return Clippers::clamp(in, -threshold, threshold);
	}

	inline static float SoftClip(const float in, float const threshold)
	{
		const float thresholdHalf = 0.5f * threshold;
		const float sign = in > 0.0f ? 1.0f : -1.0f;
		const float inAbs = in * sign;

		if (inAbs < thresholdHalf)
		{
			return sign * inAbs;
		}
		else if (inAbs < threshold + thresholdHalf)
		{
			return sign * 0.5f * (inAbs + thresholdHalf);
		}
		else
		{
			return sign * threshold;
		}
	}

	inline static float FoldBack(const float in, float const threshold)
	{
		if (in < threshold && in > -threshold)
		{
			return in;
		}
		else
		{
			return std::fabsf(std::fabsf(std::fmodf(in - threshold, 4.0f * threshold)) - 2.0f * threshold) - threshold;
		}
	}

	inline static float HalfWave(const float in, float const threshold)
	{
		/*if (in < 0.0f)
		{
			return 0.0f;
		}
		else if (in < threshold)
		{
			return in;
		}
		else
		{
			return threshold;
		}*/

		//return std::clamp(in, 0.0f, threshold);
		return Clippers::clamp(in, 0.0f, threshold);
	}

	inline static float ABS(const float in, const float threshold)
	{
		const float inAbs = std::fabsf(in);

		if (inAbs < threshold)
		{
			return inAbs;
		}
		else
		{
			return threshold;
		}
	}
};

class SlopeClipper
{
public:
	SlopeClipper() {};

	inline void init(int sampleRate)
	{
		constexpr float ratioDefault = 0.15f * 48000.0f;
		m_ratio = ratioDefault / (float)sampleRate;
	}

	inline float process(float in, float threshold)
	{
		const float out = m_inLast;

		if (in > 0.0f)
		{
			if (in > threshold)
			{
				m_inLast += (threshold - m_inLast) * m_ratio;
			}
			else
			{
				m_inLast = in;
			}
		}
		else
		{
			if (in < -threshold)
			{
				m_inLast += (-threshold - m_inLast) * m_ratio;
			}
			else
			{
				m_inLast = in;
			}
		}

		return out;
	};

private:
	float m_ratio = 0.1f;
	float m_inLast = 0.0f;
};