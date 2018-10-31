#ifndef DAISY_VECTOR3_H
#define DAISY_VECTOR3_H

#include <cmath>

namespace daisy {
	class vector3 final
	{
	public:
		float x;
		float y;
		float z;

		vector3();
		vector3(float const x, float const y, float const z);
		vector3 operator+(vector3 const& v) const;
		vector3& operator+=(vector3 const& v);
		vector3 operator-(vector3 const& v) const;
		vector3& operator-=(vector3 const& v);
		vector3 operator-() const;
		vector3 operator*(float const s) const;
		vector3& operator*=(float const s);
		vector3 operator/(float const s) const;
		vector3& operator/=(float const s);

		float length() const;
		float length_sqr() const;
		void normalize();
		vector3 normalized() const;
		float dot(vector3 const& v) const;
		vector3 cross(vector3 const& v) const;
		float distance_to(vector3 const& v) const;
		float angle_with(vector3 const& v) const;
		vector3 projection_on(vector3 const& v) const;
		vector3 perpendicular_on(vector3 const& v) const;

		static vector3 const ZERO;
		static vector3 const X_UNIT;
		static vector3 const Y_UNIT;
		static vector3 const Z_UNIT;
	};

	vector3 operator *(float const s, vector3 const& v);
}

#endif // !DAISY_VECTOR3_H
