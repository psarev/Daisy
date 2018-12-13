#ifndef DAISY_COLOR_H
#define DAISY_COLOR_H

#include <cmath>

namespace daisy {
	class color final
	{
	public:
		float r;
		float g;
		float b;

		bool operator==(color const& c) const;
		bool operator!=(color const& c) const;
		color operator*(float const s) const;
		color operator+(color const& c) const;

		static const color BLACK;
		static const color WHITE;
		static const color RED;
		static const color GREEN;
		static const color BLUE;
	};

	inline color color::operator*(float const s) const
	{
		return color{ r * s, g * s, b * s };
	}

	inline color color::operator+(color const& c) const
	{
		return color{ r + c.r, g + c.g, b + c.b };
	}

	inline bool color::operator==(color const& c) const
	{
		return (
			(std::abs(r - c.r) < 0.001f) &&
			(std::abs(g - c.g) < 0.001f) &&
			(std::abs(b - c.b) < 0.001f));
	}

	inline bool color::operator!=(color const& c) const
	{
		return !(*this == c);
	}
}

#endif // !DAISY_COLOR_H
