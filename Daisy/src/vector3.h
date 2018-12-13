#ifndef DAISY_VECTOR3_H
#define DAISY_VECTOR3_H

#include <cmath>

namespace daisy {

	template<typename T>
	class vector3 final
	{
	public:
		T x;
		T y;
		T z;

		vector3();
		vector3(T const x, T const y, T const z);
		vector3<T> operator+(vector3<T> const& v) const;
		vector3<T>& operator+=(vector3<T> const& v);
		vector3<T> operator-(vector3<T> const& v) const;
		vector3<T>& operator-=(vector3<T> const& v);
		vector3<T> operator-() const;
		vector3<T> operator*(float const s) const;
		vector3<T>& operator*=(float const s);
		vector3<T> operator/(float const s) const;
		vector3<T>& operator/=(float const s);

		float length() const;
		float length_sqr() const;
		void normalize();
		vector3<T> normalized() const;
		float dot(vector3<T> const& v) const;
		vector3<T> cross(vector3<T> const& v) const;
		float distance_to(vector3<T> const& v) const;
		float angle_with(vector3<T> const& v) const;
		vector3<T> projection_on(vector3<T> const& v) const;
		vector3<T> perpendicular_on(vector3<T> const& v) const;
	};

	template<typename T>
	vector3<T>::vector3()
		: x{}, y{}, z{}
	{
	}
	template<typename T>
	vector3<T>::vector3(T const x, T const y, T const z)
		: x(x), y(y), z(z)
	{
	}
	template<typename T>
	vector3<T> vector3<T>::operator+(vector3<T> const& v) const
	{
		return vector3<T>{x + v.x, y + v.y, z + v.z};
	}
	template<typename T>
	vector3<T>& vector3<T>::operator+=(vector3<T> const& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	template<typename T>
	vector3<T> vector3<T>::operator-(vector3<T> const& v) const
	{
		return vector3{ x - v.x, y - v.y, z - v.z };
	}
	template<typename T>
	vector3<T>& vector3<T>::operator-=(vector3<T> const& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	template<typename T>
	vector3<T> vector3<T>::operator-() const
	{
		return vector3<T>{-x, -y, -z};
	}
	template<typename T>
	vector3<T> vector3<T>::operator*(float const s) const
	{
		return vector3<T>{x * s, y * s, z * s};
	}
	template<typename T>
	vector3<T>& vector3<T>::operator*=(float const s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	template<typename T>
	vector3<T> vector3<T>::operator/(float const s) const
	{
		return vector3{ x / s, y / s, z / s };
	}
	template<typename T>
	vector3<T>& vector3<T>::operator/=(float const s)
	{
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
	template<typename T>
	float vector3<T>::length() const
	{
		return std::sqrt(x * x + y * y + z * z);
	}
	template<typename T>
	float vector3<T>::length_sqr() const
	{
		return x * x + y * y + z * z;
	}
	template<typename T>
	void vector3<T>::normalize()
	{
		float const length_reciprocal{ 1.0f / length() };
		x *= length_reciprocal;
		y *= length_reciprocal;
		z *= length_reciprocal;
	}
	template<typename T>
	vector3<T> vector3<T>::normalized() const
	{
		float const length_reciprocal{ 1.0f / length() };
		return vector3<T>{x * length_reciprocal, y * length_reciprocal, z * length_reciprocal};
	}
	template<typename T>
	float vector3<T>::distance_to(vector3<T> const& p) const
	{
		float const dist_x{ p.x - x };
		float const dist_y{ p.y - y };
		float const dist_z{ p.z - z };
		return std::sqrt(dist_x * dist_x + dist_y * dist_y + dist_z * dist_z);
	}
	template<typename T>
	float vector3<T>::angle_with(vector3<T> const& v) const
	{
		return std::acos(dot(v) / (length() * v.length()));
	}
	template<typename T>
	vector3<T> vector3<T>::projection_on(vector3<T> const& v) const
	{
		return v.normalized() * (dot(v) / v.length());
	}
	template<typename T>
	vector3<T> vector3<T>::perpendicular_on(vector3<T> const& v) const
	{
		return (*this) - projection_on(v);
	}
	template<typename T>
	float vector3<T>::dot(vector3<T> const& v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}
	template<typename T>
	vector3<T> vector3<T>::cross(vector3<T> const& v) const
	{
		return vector3<T>{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}
	template<typename T>
	vector3<T> operator*(float const s, vector3<T> const& v)
	{
		return vector3<T>{v.x * s, v.y * s, v.z * s};
	}

	typedef vector3<float> vector3f;
}

#endif // !DAISY_VECTOR3_H
