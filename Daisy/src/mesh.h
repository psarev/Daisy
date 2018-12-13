#ifndef DAISY_MESH_H
#define DAISY_MESH_H

#include <vector>
#include "mesh_attribute_info.h"
#include "vector2.h"
#include "vector3.h"
#include "color.h"

namespace daisy {
	class mesh final
	{
	public:
		mesh(std::vector<vector3f> vertices, std::vector<unsigned int> indices);

		std::vector<vector3f> const& get_vertices() const;
		std::vector<unsigned int> const& get_indices() const;

		std::vector<mesh_attribute_info<color>>& get_color_attributes();
		std::vector<mesh_attribute_info<color>> const& get_color_attributes() const;

		std::vector<mesh_attribute_info<float>>& get_float_attributes();
		std::vector<mesh_attribute_info<float>> const& get_float_attributes() const;

		std::vector<mesh_attribute_info<vector2f>>& get_vector2f_attributes();
		std::vector<mesh_attribute_info<vector2f>> const& get_vector2f_attributes() const;

		std::vector<mesh_attribute_info<vector3f>>& get_vector3f_attributes();
		std::vector<mesh_attribute_info<vector3f>> const& get_vector3f_attributes() const;

	private:
		std::vector<vector3f> const m_vertices;
		std::vector<unsigned int> const m_indices; 
		std::vector<mesh_attribute_info<color>> m_color_attributes;
		std::vector<mesh_attribute_info<float>> m_float_attributes;
		std::vector<mesh_attribute_info<vector2f>> m_vector2f_attributes;
		std::vector<mesh_attribute_info<vector3f>> m_vector3f_attributes;
	};
}

#endif // !DAISY_MESH_H
