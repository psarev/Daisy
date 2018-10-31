#ifndef DAISY_BITMAP_PAINTER_H
#define DAISY_BITMAP_PAINTER_H

#include <stddef.h>
#include "point.h"
#include "color.h"

namespace daisy {
	class bitmap_painter final
	{
	public:
		bitmap_painter(unsigned int const width, unsigned int const height);
		~bitmap_painter();

		unsigned int get_bitmap_width() const;
		unsigned int get_bitmap_height() const;
		int get_pitch() const;
		unsigned char const* get_data() const;
		color get_pixel_color(point2d const& point) const;

		void clear(unsigned char const byte_value);
		void draw_pixel(point2d const& point, color const& c);
		void draw_line(point2d const& p0, point2d const& p1, color const& c);

	private:
		unsigned int m_bitmap_width;
		unsigned int m_bitmap_height;
		size_t m_data_total_size;
		unsigned char* m_data;
		unsigned int m_pitch;
	};
}

#endif // !DAISY_BITMAP_PAINTER_H
