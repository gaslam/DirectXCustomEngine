#pragma once
#include <cmath>

namespace MathUtils
{
	inline bool IsEqual(const float n1,const float n2, const float epsilon = 0.1f)
	{
		const float result{ fabs(n1 - n2) };
		if(result < epsilon)
		{
			return true;
		}
		return false;
	}
}
