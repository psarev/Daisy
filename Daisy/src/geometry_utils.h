#ifndef DAISY_GEOMETRY_UTILS_H
#define DAISY_GEOMETRY_UTILS_H

#include <cmath>
#include "vector2.h"

namespace daisy {
	class line final
	{
	public:
		float a;
		float b;
		float c;

		line(float const a_coeff, float const b_coeff, float const c_coeff)
			: a(a_coeff), b(b_coeff), c(c_coeff)
		{
		}

		line(float const x0, float const y0, float const x1, float const y1)
		{
			a = -(y1 - y0);
			b = x1 - x0;
			c = (y1 - y0) * x0 - (x1 - x0) * y0;
		}

		float at(float const x, float const y) const
		{
			return x * a + y * b + c;
		}

		vector2f intersection(line const& line) const
		{
			float const y{ (line.a * c - a * line.c) / (a * line.b - line.a * b) };
			float const x{ a == 0.0f ? ((line.b * c - b * line.c) / (line.a * b)) : (-(b / a) * y - (c / a)) };

			return vector2f{ x,y };
		}
	};

	template<typename TPoint>
	class aabb final
	{
	public:
		TPoint from;
		TPoint to;
	};

	inline float triangle_2d_area(
		float const x0, float const y0,
		float const x1, float const y1,
		float const x2, float const y2)
	{
		return std::abs(0.5f * (x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)));
	}
}

#endif // !DAISY_GEOMETRY_UTILS_H