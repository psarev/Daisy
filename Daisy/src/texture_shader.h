#ifndef DAISY_TEXTURE_SHADER_H
#define DAISY_TEXTURE_SHADER_H

#include <vector>
#include "shader.h"
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh_attribute_info.h"
#include "texture.h"

namespace daisy {
	class texture_shader final
	{
	public:
		std::vector<shader_bind_point_info<color>> get_color_bind_points();
		std::vector<shader_bind_point_info<float>> get_float_bind_points();
		std::vector<shader_bind_point_info<vector2f>> get_vector2f_bind_points();
		std::vector<shader_bind_point_info<vector3f>> get_vector3f_bind_points();

		vector4 process_vertex(vector4 const& vertex);
		color process_pixel(vector2ui const& pixel);

		void set_mvp_matrix(matrix4x4 const& mvp);
		void set_texture(texture* tex);
		
	private:
		vector2f m_uv;
		matrix4x4 m_mvp;
		texture* m_texture;
	};

	inline void texture_shader::set_mvp_matrix(matrix4x4 const& mvp)
	{
		m_mvp = mvp;
	}

	inline void texture_shader::set_texture(texture* tex)
	{
		m_texture = tex;
	}

	inline vector4 texture_shader::process_vertex(vector4 const& vertex)
	{
		return vertex * m_mvp;
	}

	inline color texture_shader::process_pixel(vector2ui const& pixel)
	{
		//No filter, using nearest neighbour
		return m_texture->get_pixel_color(
			vector2ui{
				static_cast<unsigned int>(m_texture->get_width() * m_uv.x),
				static_cast<unsigned int>(m_texture->get_height() * m_uv.y)
			}
		);
	}

	inline std::vector<shader_bind_point_info<color>> texture_shader::get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{};
	}

	inline std::vector<shader_bind_point_info<float>> texture_shader::get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}

	inline std::vector<shader_bind_point_info<vector2f>> texture_shader::get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{shader_bind_point_info<vector2f>{TEXCOORD_ATTR_ID, &m_uv}};
	}

	inline std::vector<shader_bind_point_info<vector3f>> texture_shader::get_vector3f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3f>>{};
	}
}

#endif // !DAISY_TEXTURE_SHADER_H
