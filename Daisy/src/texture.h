#ifndef DAISY_TEXTURE_H
#define DAISY_TEXTURE_H

#include <cstring>
#include <cstddef>
#include <string>
#include "vector2.h"
#include "color.h"

namespace daisy {
	class texture final
	{
	public:
		texture(unsigned int const width, unsigned int const height);
		texture(texture&& other);
		~texture();

		unsigned int get_width() const;
		unsigned int get_height() const;

		unsigned int get_pitch() const;

		unsigned char const* get_data() const;

		color get_pixel_color(vector2ui const& point) const;
		void set_pixel_color(vector2ui const& point, color const& color);
	
		void clear(unsigned char const byte_values);

		static texture load_from_file(std::string file);
	private:
		unsigned int m_width;
		unsigned int m_height;

		size_t m_data_total_size;
		unsigned char* m_data;

		unsigned int m_pitch;
	};

	inline void texture::set_pixel_color(vector2ui const& point, color const& color)
	{
		unsigned int const pixel_first_byte_index{ m_pitch * point.y + point.x * 4 };

		m_data[pixel_first_byte_index + 0] = static_cast<unsigned char>(color.b * 255);
		m_data[pixel_first_byte_index + 1] = static_cast<unsigned char>(color.g * 255);
		m_data[pixel_first_byte_index + 2] = static_cast<unsigned char>(color.r * 255);

	}

	inline void texture::clear(unsigned char const byte_values)
	{
		memset(m_data, byte_values, m_data_total_size);
	}
}

#endif // !DAISY_TEXTURE_H
