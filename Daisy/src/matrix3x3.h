#ifndef DAISY_MATRIX3X3_H
#define DAISY_MATRIX3X3_H

#include <cmath>
#include "vector3.h"

namespace daisy {
	class matrix3x3 final
	{
	public:
		float values[3][3];

		matrix3x3();
		matrix3x3(float const m[3][3]);
		matrix3x3(
			float const m00, float const m01, float const m02,
			float const m10, float const m11, float const m12,
			float const m20, float const m21, float const m22
		);

		matrix3x3 operator*(matrix3x3 const& m) const;

		float det() const;

		matrix3x3 inversed() const;

		matrix3x3 inversed_precalc_det(float const det) const;

		static matrix3x3 scale(float const x, float const y, float const z);
		static matrix3x3 uniform_scale(float const s);
		static matrix3x3 rotation_around_x_axis(float const radians);
		static matrix3x3 rotation_around_y_axis(float const radians);
		static matrix3x3 rotation_around_z_axis(float const radians);
		static matrix3x3 rotation_around_axis(vector3f const& axis, float const radians);

		static const matrix3x3 IDENTITY;
	};

	template<typename T>
	vector3<T> operator*(vector3<T> const& v, matrix3x3 const& m)
	{
		return vector3<T>{
			v.x * m.values[0][0] + v.y * m.values[1][0] + v.z * m.values[2][0],
				v.x * m.values[0][1] + v.y * m.values[1][1] + v.z * m.values[2][1],
				v.x * m.values[0][2] + v.y * m.values[1][2] + v.z * m.values[2][2]
		};
	}
}

#endif // !DAISY_MATRIX3X3_H