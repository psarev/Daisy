#ifndef DAISY_COLOR_SHADER_H
#define DAISY_COLOR_SHADER_H

#include <vector>
#include "shader.h"
#include "color.h"
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4x4.h"
#include "mesh_attribute_info.h"

namespace daisy {
	class color_shader final
	{
	public:
		std::vector<shader_bind_point_info<color>> get_color_bind_points();
		std::vector<shader_bind_point_info<float>> get_float_bind_points();
		std::vector<shader_bind_point_info<vector2f>> get_vector2f_bind_points();
		std::vector<shader_bind_point_info<vector3f>> get_vector3f_bind_points();

		vector4 process_vertex(vector4 const& vertex);
		color process_pixel(vector2ui const& pixel);

		void set_mvp_matrix(matrix4x4 const& mvp);
		
	private:
		matrix4x4 m_mvp;
		color m_color;
	};

	inline void color_shader::set_mvp_matrix(matrix4x4 const& mvp)
	{
		m_mvp = mvp;
	}

	inline vector4 color_shader::process_vertex(vector4 const& vertex)
	{
		return vertex * m_mvp;
	}

	inline color color_shader::process_pixel(vector2ui const& pixel)
	{
		return m_color;
	}

	inline std::vector<shader_bind_point_info<color>> color_shader::get_color_bind_points()
	{
		return std::vector<shader_bind_point_info<color>>{
			shader_bind_point_info<color> { VERTEX_COLOR_ATTR_ID, &m_color }};
	}

	inline std::vector<shader_bind_point_info<float>> color_shader::get_float_bind_points()
	{
		return std::vector<shader_bind_point_info<float>>{};
	}

	inline std::vector<shader_bind_point_info<vector2f>> color_shader::get_vector2f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector2f>>{};
	}

	inline std::vector<shader_bind_point_info<vector3f>> color_shader::get_vector3f_bind_points()
	{
		return std::vector<shader_bind_point_info<vector3f>>{};
	}
}

#endif // !DAISY_COLOR_SHADER_H
