#ifndef DAISY_MESH_ATTRIBUTE_INFO_H
#define DAISY_MESH_ATTRIBUTE_INFO_H

#include <vector>

namespace daisy {

	enum class attribute_interpolation_option
	{
		linear,
		perspective_correct
	};

	template<typename TAttr>
	class mesh_attribute_info final
	{
	public:
		mesh_attribute_info(
			unsigned int const attribute_id,
			std::vector<TAttr> const data,
			std::vector<unsigned int> const indices,
			attribute_interpolation_option interpolation_option);

		unsigned int get_id() const;

		std::vector<TAttr> const& get_data() const;

		std::vector<unsigned int> const& get_indices() const;

		attribute_interpolation_option get_interpolation_option() const;

	private:
		unsigned int const m_id;
		std::vector<TAttr> const m_data;
		std::vector<unsigned int> const m_indices;
		attribute_interpolation_option m_interpolation_option;
	};

	unsigned int const VERTEX_COLOR_ATTR_ID = 0;
	unsigned int const TEXCOORD_ATTR_ID = 1;
	unsigned int const NORMAL_ATTR_ID = 2;

	template<typename TAttr>
	mesh_attribute_info<TAttr>::mesh_attribute_info(
		unsigned int const attribute_id, 
		std::vector<TAttr> const data,
		std::vector<unsigned int> const indices,
		attribute_interpolation_option interpolation_option) 
		: m_id(attribute_id),
		  m_data(data),
		  m_indices(indices),
		  m_interpolation_option(interpolation_option)
	{
	}

	template<typename TAttr>
	unsigned int mesh_attribute_info<TAttr>::get_id() const
	{
		return m_id;
	}

	template<typename TAttr>
	std::vector<TAttr> const& mesh_attribute_info<TAttr>::get_data() const
	{
		return m_data;
	}

	template<typename TAttr>
	std::vector<unsigned int> const& mesh_attribute_info<TAttr>::get_indices() const
	{
		return m_indices;
	}

	template<typename TAttr>
	attribute_interpolation_option mesh_attribute_info<TAttr>::get_interpolation_option() const
	{
		return m_interpolation_option;
	}
}

#endif // !DAISY_MESH_ATTRIBUTE_INFO_H
