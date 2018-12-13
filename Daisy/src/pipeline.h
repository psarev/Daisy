#ifndef DAISY_PIPELINE_H
#define DAISY_PIPELINE_H

#include <limits>
#include <algorithm>
#include <stdexcept>
#include "vector4.h"
#include "matrix4x4.h"
#include "matrix3x3.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "geometry_utils.h"

namespace daisy {

	template<typename TAttr>

	class binded_mesh_attribute_info final
	{
	public:
		mesh_attribute_info<TAttr> const& info;
		TAttr* bind_point;
	};

	enum class face_culling_option
	{
		clockwise,
		counter_clockwise
	};

	enum class rasterization_algorithm_option
	{
		traversal_aabb,
		traversal_backtracking,
		traversal_zigzag,
		inversed_slope,
		homogeneous
	};

	enum class fill_mode_option
	{
		solid,
		wireframe
	};

	class pipeline final
	{
	public:
		pipeline();

		void set_rasterization_algorithm(rasterization_algorithm_option value);

		void set_face_culling(face_culling_option value);

		void set_fill_mode(fill_mode_option value);

		template<typename TShader>
		void draw(mesh const& mesh, TShader& shader, texture& target_texture);

	private:
		template<typename TAttr>
		void bind_attributes(
			std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
			std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
			std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage
		);

		template<typename TShader>
		void rasterize_traversal_aabb(
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture
		);

		template<typename TShader>
		void rasterize_traversal_backtracking(
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture
		);

		template<typename TShader>
		void rasterize_traversal_zigzag(
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture
		);

		template<typename TShader>
		void rasterize_inversed_slope(
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture
		);

		template<typename TShader>
		void rasterize_inversed_slope_top_or_bottom_triangle(
			unsigned int index0,
			unsigned int index1,
			unsigned int index2,
			vector4 v0, vector4 v1, vector4 v2,
			float const v0_v1_edge_distance_offset,
			float const v0_v2_edge_distance_offset,
			TShader& shader, texture& target_texture
		);

		template<typename TShader>
		void rasterize_homogeneous(
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			vector4 const& v0, vector4 const& v1, vector4 const& v2,
			TShader& shader, texture& target_texture
		);

		template<typename TAttr>
		void save_edges_coefficients(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			std::vector<vector3<TAttr>>& coefficients_storage,
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			matrix3x3 const& vertices_matrix_inversed
		);

		template<typename TAttr>
		void set_bind_points_values_from_edge_coefficients(
			std::vector<binded_mesh_attribute_info<TAttr>>& binds,
			std::vector<vector3<TAttr>> const& coefficients_storage,
			vector2f const& point,
			float const w
		);

		template<typename TAttr>
		void set_bind_points_values_from_scanline_endpoints(
			std::vector<binded_mesh_attribute_info<TAttr>>& binds,
			std::vector<TAttr> const& left_endpoint_values,
			std::vector<TAttr> const& right_endpoint_values,
			float const scanline_distance_normalized,
			float const zview_reciprocal_left, float const zview_reciprocal_right
		);

		template<typename TAttr>
		void save_intermediate_attrs_values(
			std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
			unsigned int const top_vertex_index,
			unsigned int const left_vertex_index,
			unsigned int const right_vertex_index,
			float const distance_from_top_to_left_normalized,
			float const distance_from_top_to_right_normalized,
			std::vector<TAttr>& left_values_storage,
			std::vector<TAttr>& right_values_storage,
			float const top_vertex_zview_reciprocal,
			float const left_vertex_zview_reciprocal,
			float const right_vertex_zview_reciprocal
		);

		bool is_point_on_positive_halfspace_top_left(
			float const edge_equation_value,
			float const edge_equation_value_a,
			float const edge_equation_value_b
		) const;

		float get_next_pixel_center_exclusive(float const f);
		float get_next_pixel_center_inclusive(float const f);
		float get_previous_pixel_center_exclusive(float const f);

		template<typename TAttr>
		void set_bind_points_values_from_barycentric(
			std::vector<binded_mesh_attribute_info<TAttr>>& binds,
			unsigned int const index0,
			unsigned int const index1,
			unsigned int const index2,
			float const b0, float const b1, float const b2,
			float const z0_view_space_reciprocal,
			float const z1_view_space_reciprocal,
			float const z2_view_space_reciprocal
		);

		rasterization_algorithm_option m_rasterization_algorithm;
		face_culling_option m_face_culling;
		fill_mode_option m_fill_mode;

		std::vector<binded_mesh_attribute_info<color>> m_binded_color_attributes;
		std::vector<binded_mesh_attribute_info<float>> m_binded_float_attributes;
		std::vector<binded_mesh_attribute_info<vector2f>> m_binded_vector2f_attributes;
		std::vector<binded_mesh_attribute_info<vector3f>> m_binded_vector3f_attributes;

		std::vector<vector4> m_transformed_vertices_storage;

		std::vector<bool> m_transformed_vertices_clip_flags_storage;

		float const EPSILON = 0.0001f;
	};

	template<typename TShader>
	void pipeline::draw(mesh const& mesh, TShader& shader, texture& target_texture)
	{
		bind_attributes(shader.get_color_bind_points(), mesh.get_color_attributes(),
			m_binded_color_attributes);
		bind_attributes(shader.get_float_bind_points(), mesh.get_float_attributes(),
			m_binded_float_attributes);
		bind_attributes(shader.get_vector2f_bind_points(), mesh.get_vector2f_attributes(),
			m_binded_vector2f_attributes);
		bind_attributes(shader.get_vector3f_bind_points(), mesh.get_vector3f_attributes(),
			m_binded_vector3f_attributes);

		std::vector<vector3f> const& vertices = mesh.get_vertices();
		size_t vertices_count{ vertices.size() };

		if (m_transformed_vertices_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_storage.reserve(vertices_count);
		}

		m_transformed_vertices_storage.clear();

		if (m_transformed_vertices_clip_flags_storage.capacity() < vertices_count)
		{
			m_transformed_vertices_clip_flags_storage.reserve(vertices_count);
		}

		m_transformed_vertices_clip_flags_storage.clear();

		for (size_t i{ 0 }; i < vertices_count; ++i)
		{
			vector3f const& v{ vertices.at(i) };
			vector4 v_transformed{ shader.process_vertex(vector4{v.x, v.y, v.z, 1.0f}) };

			bool clipped{ false };
			if ((v_transformed.x > v_transformed.w) || (v_transformed.x < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.y > v_transformed.w) || (v_transformed.y < -v_transformed.w))
			{
				clipped = true;
			}
			else if ((v_transformed.z > v_transformed.w) || (v_transformed.z < -v_transformed.w))
			{
				clipped = true;
			}

			m_transformed_vertices_storage.push_back(v_transformed);
			m_transformed_vertices_clip_flags_storage.push_back(clipped);
		}

		float const width{ static_cast<float>(target_texture.get_width()) };
		float const height{ static_cast<float>(target_texture.get_height()) };

		if (m_rasterization_algorithm == rasterization_algorithm_option::homogeneous)
		{
			matrix4x4 const ndc_to_screen{
				(width) / 2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, -(height) / 2.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				(width) / 2.0f, (height) / 2.0f, 0.0f, 1.0f };

			for (size_t i{ 0 }; i < vertices_count; ++i)
			{
				vector4& v = m_transformed_vertices_storage.at(i);
				v = v * ndc_to_screen;
			}
		}
		else
		{
			for (size_t i{ 0 }; i < vertices_count; ++i)
			{
				if (m_transformed_vertices_clip_flags_storage.at(i) == true)
				{
					continue;
				}

				vector4& v = m_transformed_vertices_storage.at(i);

				float const w_inversed{ 1.0f / v.w };
				v.x *= w_inversed;
				v.y *= w_inversed;
				v.z *= w_inversed;
				v.w = w_inversed;

				// NDC to screen
				v.x = v.x * width / 2.0f + width / 2.0f;
				v.y = -v.y * height / 2.0f + height / 2.0f;
			}
		}

		//
		// Rasterization
		//

		std::vector<unsigned int> const& indices = mesh.get_indices();

		size_t indices_count{ indices.size() };

		for (size_t i{ 0 }; i < indices_count; i += 3)
		{
			unsigned int const index0{ indices.at(i + 0) };
			unsigned int const index1{ indices.at(i + 1) };
			unsigned int const index2{ indices.at(i + 2) };

			vector4 const& v0 = m_transformed_vertices_storage.at(index0);
			vector4 const& v1 = m_transformed_vertices_storage.at(index1);
			vector4 const& v2 = m_transformed_vertices_storage.at(index2);

			bool const v0_clipped{ m_transformed_vertices_clip_flags_storage.at(index0) };
			bool const v1_clipped{ m_transformed_vertices_clip_flags_storage.at(index1) };
			bool const v2_clipped{ m_transformed_vertices_clip_flags_storage.at(index2) };

			if (m_rasterization_algorithm != rasterization_algorithm_option::homogeneous)
			{
				// TODO: clipping
				if (v0_clipped || v1_clipped || v2_clipped)
				{
					continue;
				}
			}

			if (m_fill_mode == fill_mode_option::wireframe)
			{
				// TODO
			}
			else
			{
				switch (m_rasterization_algorithm)
				{
				case rasterization_algorithm_option::traversal_aabb:
					rasterize_traversal_aabb(
						index0, index1, index2,
						v0, v1, v2,
						shader, target_texture
					);
					break;
				case rasterization_algorithm_option::traversal_backtracking:
					rasterize_traversal_backtracking(
						index0, index1, index2,
						v0, v1, v2,
						shader, target_texture
					);
					break;
				case rasterization_algorithm_option::traversal_zigzag:
					rasterize_traversal_zigzag(
						index0, index1, index2,
						v0, v1, v2,
						shader, target_texture
					);
					break;
				case rasterization_algorithm_option::inversed_slope:
					rasterize_inversed_slope(
						index0, index1, index2,
						v0, v1, v2,
						shader, target_texture
					);
					break;
				case rasterization_algorithm_option::homogeneous:
					rasterize_homogeneous(
						index0, index1, index2,
						v0, v1, v2,
						shader, target_texture
					);
					break;
				}
			}
		}
	}

	template<typename TAttr>
	void pipeline::bind_attributes(
		std::vector<shader_bind_point_info<TAttr>> const& required_bind_points,
		std::vector<mesh_attribute_info<TAttr>> const& available_attributes,
		std::vector<binded_mesh_attribute_info<TAttr>>& binded_attributes_storage
	)
	{
		binded_attributes_storage.clear();

		size_t const bind_points_size{ required_bind_points.size() };
		size_t const available_attributes_size{ available_attributes.size() };

		for (size_t i{ 0 }; i < bind_points_size; ++i)
		{
			shader_bind_point_info<TAttr> const& bind_point_info = required_bind_points.at(i);

			bool binded{ false };

			for (size_t j{ 0 }; j < available_attributes_size; ++j)
			{
				mesh_attribute_info<TAttr> const& attr_info = available_attributes.at(j);

				if (attr_info.get_id() == bind_point_info.attribute_id)
				{
					binded_attributes_storage.push_back(binded_mesh_attribute_info<TAttr>{attr_info, bind_point_info.bind_point});

					binded = true;
					break;
				}
			}

			if (!binded)
			{
				throw std::runtime_error("Mesh doesn't contain attribute required by shader");
			}
		}
	};

	template<typename TShader>
	void pipeline::rasterize_traversal_aabb(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		line edge0{ v1.x, v1.y, v0.x, v0.y };
		line edge1{ v2.x, v2.y, v1.x, v1.y };
		line edge2{ v0.x, v0.y, v2.x, v2.y };

		if (m_face_culling == face_culling_option::clockwise)
		{
			edge0.a *= -1.0f;
			edge0.b *= -1.0f;
			edge0.c *= -1.0f;

			edge1.a *= -1.0f;
			edge1.b *= -1.0f;
			edge1.c *= -1.0f;

			edge2.a *= -1.0f;
			edge2.b *= -1.0f;
			edge2.c *= -1.0f;
		}

		//Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		aabb<vector2ui> bounding_box{
			vector2ui{
				static_cast<unsigned int>(std::min(std::min(v0.x, v1.x), v2.x)),
				static_cast<unsigned int>(std::min(std::min(v0.y, v1.y), v2.y))},
			vector2ui{
				static_cast<unsigned int>(std::max(std::max(v0.x, v1.x), v2.x)),
				static_cast<unsigned int>(std::max(std::max(v0.y, v1.y), v2.y))}
		};

		for (unsigned int y{ bounding_box.from.y }; y <= bounding_box.to.y; ++y)
		{
			float const pixel_center_y{ static_cast<float>(y) + 0.5f };

			float const first_x_center{ static_cast<float>(bounding_box.from.x) + 0.5f };

			float edge0_equation_value(edge0.at(first_x_center, pixel_center_y));
			float edge1_equation_value(edge1.at(first_x_center, pixel_center_y));
			float edge2_equation_value(edge2.at(first_x_center, pixel_center_y));

			for (unsigned int x{ bounding_box.from.x }; x <= bounding_box.to.x; ++x)
			{
				float const pixel_center_x{ static_cast<float>(x) + 0.5f };

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{ triangle_2d_area(v0.x, v0.y, v1.x, v1.y, pixel_center_x, pixel_center_y) };
					float const area12{ triangle_2d_area(v1.x, v1.y, v2.x, v2.y, pixel_center_x, pixel_center_y) };

					// Calculate barycentric coordinates
					float const b2{ area01 * triangle_area_inversed };
					float const b0{ area12 * triangle_area_inversed };
					float const b1{ 1.0f - b0 - b2 };

					// Process different attributes
					set_bind_points_values_from_barycentric<color>(
						m_binded_color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<float>(
						m_binded_float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector2f>(
						m_binded_vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector3f>(
						m_binded_vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					// Pass pixel to shader
					vector2ui point_ui{ x, y };
					target_texture.set_pixel_color(point_ui, shader.process_pixel(point_ui));
				}

				edge0_equation_value += edge0.a;
				edge1_equation_value += edge1.a;
				edge2_equation_value += edge2.a;
			}
		}
	}

	template<typename TShader>
	void pipeline::rasterize_traversal_backtracking(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		line edge0{ v1.x, v1.y, v0.x, v0.y };
		line edge1{ v2.x, v2.y, v1.x, v1.y };
		line edge2{ v0.x, v0.y, v2.x, v2.y };

		if (m_face_culling == face_culling_option::clockwise)
		{
			edge0.a *= -1.0f;
			edge0.b *= -1.0f;
			edge0.c *= -1.0f;

			edge1.a *= -1.0f;
			edge1.b *= -1.0f;
			edge1.c *= -1.0f;

			edge2.a *= -1.0f;
			edge2.b *= -1.0f;
			edge2.c *= -1.0f;
		}

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		vector4 v0_sorted{ v0 };
		vector4 v1_sorted{ v1 };
		vector4 v2_sorted{ v2 };

		// Sort vertices by y-coordinate
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v0_sorted, v1_sorted);
		}
		if (v2_sorted.y < v1_sorted.y)
		{
			std::swap(v2_sorted, v1_sorted);
		}
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v1_sorted, v0_sorted);
		}
		// v0 is top vertex
		// v2 is bottom vertex
		vector2ui current_pixel{ static_cast<unsigned int>(v0_sorted.x), static_cast<unsigned int>(v0_sorted.y) };
		vector2f current_pixel_center{ std::floor(v0_sorted.x) + 0.5f, std::floor(v0_sorted.y) + 0.5f };
		float edge0_equation_value{ edge0.at(current_pixel_center.x, current_pixel_center.y) };
		float edge1_equation_value{ edge1.at(current_pixel_center.x, current_pixel_center.y) };
		float edge2_equation_value{ edge2.at(current_pixel_center.x, current_pixel_center.y) };
		while (current_pixel_center.y <= v2_sorted.y)
		{

			// Backtracking
			while (true)
			{
				if ((edge0_normal_pointing_right && edge0_equation_value < 0) ||
					(edge1_normal_pointing_right && edge1_equation_value < 0) ||
					(edge2_normal_pointing_right && edge2_equation_value < 0))
				{
					break;
				}
				current_pixel.x -= 1;
				current_pixel_center.x -= 1.0f;
				edge0_equation_value -= edge0.a;
				edge1_equation_value -= edge1.a;
				edge2_equation_value -= edge2.a;
			}

			// Moving along the scanline
			while (true)
			{
				if ((!edge0_normal_pointing_right && edge0_equation_value < 0) ||
					(!edge1_normal_pointing_right && edge1_equation_value < 0) ||
					(!edge2_normal_pointing_right && edge2_equation_value < 0))
				{
					break;
				}
				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{ triangle_2d_area(v0.x, v0.y, v1.x, v1.y, current_pixel_center.x, current_pixel_center.y) };
					float const area12{ triangle_2d_area(v1.x, v1.y, v2.x, v2.y, current_pixel_center.x, current_pixel_center.y) };

					// Calculate barycentric coordinates
					float const b2{ area01 * triangle_area_inversed };
					float const b0{ area12 * triangle_area_inversed };
					float const b1{ 1.0f - b0 - b2 };

					// Process different attributes
					set_bind_points_values_from_barycentric<color>(
						m_binded_color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<float>(
						m_binded_float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector2f>(
						m_binded_vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector3f>(
						m_binded_vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					// Pass pixel to shader
					target_texture.set_pixel_color(current_pixel, shader.process_pixel(current_pixel));
				}
				current_pixel.x += 1;
				current_pixel_center.x += 1.0f;
				edge0_equation_value += edge0.a;
				edge1_equation_value += edge1.a;
				edge2_equation_value += edge2.a;
			}
			current_pixel.y += 1;
			current_pixel_center.y += 1.0f;
			edge0_equation_value += edge0.b;
			edge1_equation_value += edge1.b;
			edge2_equation_value += edge2.b;
		}
	}

	template<typename TShader>
	void pipeline::rasterize_traversal_zigzag(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		line edge0{ v1.x, v1.y, v0.x, v0.y };
		line edge1{ v2.x, v2.y, v1.x, v1.y };
		line edge2{ v0.x, v0.y, v2.x, v2.y };

		if (m_face_culling == face_culling_option::clockwise)
		{
			edge0.a *= -1.0f;
			edge0.b *= -1.0f;
			edge0.c *= -1.0f;

			edge1.a *= -1.0f;
			edge1.b *= -1.0f;
			edge1.c *= -1.0f;

			edge2.a *= -1.0f;
			edge2.b *= -1.0f;
			edge2.c *= -1.0f;
		}

		bool const edge0_normal_pointing_right = edge0.a > 0;
		bool const edge1_normal_pointing_right = edge1.a > 0;
		bool const edge2_normal_pointing_right = edge2.a > 0;

		// Calculate triangle area on screen and inverse it
		float const triangle_area_inversed = 1.0f / triangle_2d_area(v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

		vector4 v0_sorted{ v0 };
		vector4 v1_sorted{ v1 };
		vector4 v2_sorted{ v2 };

		// Sort vertices by y-coordinate
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v0_sorted, v1_sorted);
		}
		if (v2_sorted.y < v1_sorted.y)
		{
			std::swap(v2_sorted, v1_sorted);
		}
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v1_sorted, v0_sorted);
		}
		// v0 is top vertex
		// v2 is bottom vertex
		vector2ui current_pixel{ static_cast<unsigned int>(v0_sorted.x), static_cast<unsigned int>(v0_sorted.y) };
		vector2f current_pixel_center{ std::floor(v0_sorted.x) + 0.5f, std::floor(v0_sorted.y) + 0.5f };
		float edge0_equation_value{ edge0.at(current_pixel_center.x, current_pixel_center.y) };
		float edge1_equation_value{ edge1.at(current_pixel_center.x, current_pixel_center.y) };
		float edge2_equation_value{ edge2.at(current_pixel_center.x, current_pixel_center.y) };

		bool start_direction_is_right{ true };
		bool is_moving_right{ true };

		unsigned int zigzag_x_index{ current_pixel.x - 1 };

		while (current_pixel_center.y <= v2_sorted.y)
		{
			while (true)
			{
				if (is_moving_right)
				{
					// If we're moving right, we need to check pixel for being outside of a triangle from the right side
					if ((!edge0_normal_pointing_right && edge0_equation_value < 0) ||
						(!edge1_normal_pointing_right && edge1_equation_value < 0) ||
						(!edge2_normal_pointing_right && edge2_equation_value < 0))
					{
						// If we moved only in side, switch direction and move to the first pixel we didn't visit yet
						if (start_direction_is_right)
						{
							is_moving_right = false;
							int const delta = current_pixel.x - zigzag_x_index;

							current_pixel.x -= delta;
							current_pixel_center.x -= delta * 1.0f;

							edge0_equation_value -= delta * edge0.a;
							edge1_equation_value -= delta * edge1.a;
							edge2_equation_value -= delta * edge2.a;

							continue;
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					// If we're moving left, we need to check pixel for being outside of a triangle from the left side
					if ((edge0_normal_pointing_right && edge0_equation_value < 0) ||
						(edge1_normal_pointing_right && edge1_equation_value < 0) ||
						(edge2_normal_pointing_right && edge2_equation_value < 0))
					{
						// If we moved only in side, switch direction and move to the first pixel we didn't visit yet
						if (!start_direction_is_right)
						{
							is_moving_right = true;

							int const delta = zigzag_x_index - current_pixel.x;

							current_pixel.x += delta;
							current_pixel_center.x += delta * 1.0f;

							edge0_equation_value += delta * edge0.a;
							edge1_equation_value += delta * edge1.a;
							edge2_equation_value += delta * edge2.a;

							continue;
						}
						else
						{
							break;
						}
					}
				}

				if (is_point_on_positive_halfspace_top_left(edge0_equation_value, edge0.a, edge0.b) &&
					is_point_on_positive_halfspace_top_left(edge1_equation_value, edge1.a, edge1.b) &&
					is_point_on_positive_halfspace_top_left(edge2_equation_value, edge2.a, edge2.b))
				{
					float const area01{ triangle_2d_area(v0.x, v0.y, v1.x, v1.y, current_pixel_center.x, current_pixel_center.y) };
					float const area12{ triangle_2d_area(v1.x, v1.y, v2.x, v2.y, current_pixel_center.x, current_pixel_center.y) };

					float const b2{ area01 * triangle_area_inversed };
					float const b0{ area12 * triangle_area_inversed };
					float const b1{ 1.0f - b0 - b2 };

					set_bind_points_values_from_barycentric<color>(
						m_binded_color_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<float>(
						m_binded_float_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector2f>(
						m_binded_vector2f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);
					set_bind_points_values_from_barycentric<vector3f>(
						m_binded_vector3f_attributes,
						index0, index1, index2,
						b0, b1, b2,
						v0.w, v1.w, v2.w);

					target_texture.set_pixel_color(current_pixel, shader.process_pixel(current_pixel));
				}

				if (is_moving_right)
				{
					current_pixel.x += 1;
					current_pixel_center.x += 1.0f;

					edge0_equation_value += edge0.a;
					edge1_equation_value += edge1.a;
					edge2_equation_value += edge2.a;
				}
				else
				{
					current_pixel.x -= 1;
					current_pixel_center.x -= 1.0f;

					edge0_equation_value -= edge0.a;
					edge1_equation_value -= edge1.a;
					edge2_equation_value -= edge2.a;
				}
			}

			current_pixel.y += 1;
			current_pixel_center.y += 1.0f;

			edge0_equation_value += edge0.b;
			edge1_equation_value += edge1.b;
			edge2_equation_value += edge2.b;

			start_direction_is_right = true;
			is_moving_right = true;
			zigzag_x_index = current_pixel.x - 1;
		}
	}

	inline bool pipeline::is_point_on_positive_halfspace_top_left(float const edge_equation_value, float const edge_equation_a, float const edge_equation_b) const
	{
		if (std::abs(edge_equation_value) < EPSILON)
		{
			if (std::abs(edge_equation_a) < EPSILON)
			{
				return edge_equation_b > 0.0f;
			}
			else
			{
				return edge_equation_a > 0.0f;
			}
		}
		else
		{
			return edge_equation_value > 0.0f;
		}
	}

	template<typename TAttr>
	void pipeline::set_bind_points_values_from_barycentric(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		float const b0, float const b1, float const b2,
		float const z0_view_space_reciprocal, 
		float const z1_view_space_reciprocal,
		float const z2_view_space_reciprocal
	)
	{
		size_t binds_count{ binds.size() };
		for (size_t i{ 0 }; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				(*binded_attr.bind_point) = value0 * b0 + value1 * b1 + value2 * b2;
			}
			else if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::perspective_correct)
			{
				TAttr const value0_div_zview = value0 * z0_view_space_reciprocal;
				TAttr const value1_div_zview = value1 * z1_view_space_reciprocal;
				TAttr const value2_div_zview = value2 * z2_view_space_reciprocal;
				float const zview_reciprocal_interpolated = z0_view_space_reciprocal * b0 + z1_view_space_reciprocal * b1 + z2_view_space_reciprocal * b2;
				TAttr value_div_zview_interpolated = value0_div_zview * b0 + value1_div_zview * b1 + value2_div_zview * b2;
				(*binded_attr.bind_point) = value_div_zview_interpolated * (1.0f / zview_reciprocal_interpolated);
			}
		}
	}

	template<typename TShader>
	void pipeline::rasterize_inversed_slope(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		// Sort vertices by y-coordinate
		vector4 v0_sorted{ v0 };
		vector4 v1_sorted{ v1 };
		vector4 v2_sorted{ v2 };
		unsigned int index0_sorted{ index0 };
		unsigned int index1_sorted{ index1 };
		unsigned int index2_sorted{ index2 };
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v0_sorted, v1_sorted);
			std::swap(index0_sorted, index1_sorted);
		}
		if (v2_sorted.y < v1_sorted.y)
		{
			std::swap(v2_sorted, v1_sorted);
			std::swap(index2_sorted, index1_sorted);
		}
		if (v1_sorted.y < v0_sorted.y)
		{
			std::swap(v1_sorted, v0_sorted);
			std::swap(index1_sorted, index0_sorted);
		}
		// v0 is top vertex
		// v2 is bottom vertex
		// Check if it's a top, bottom or custom triangle
		//
		if (std::abs(v1_sorted.y - v2_sorted.y) < EPSILON)
		{
			// Top triangle
			//
			if (v2_sorted.x < v1_sorted.x)
			{
				std::swap(v2_sorted, v1_sorted);
				std::swap(index2_sorted, index1_sorted);
			}
			rasterize_inversed_slope_top_or_bottom_triangle(
				index0_sorted, index1_sorted, index2_sorted,
				v0_sorted, v1_sorted, v2_sorted,
				0.0f, 0.0f,
				shader, target_texture);
		}
		else if (std::abs(v0_sorted.y - v1_sorted.y) < EPSILON)
		{
			// Bottom triangle
			//
			if (v1_sorted.x < v0_sorted.x)
			{
				std::swap(v1_sorted, v0_sorted);
				std::swap(index1_sorted, index0_sorted);
			}
			rasterize_inversed_slope_top_or_bottom_triangle(
				index2_sorted, index0_sorted, index1_sorted,
				v2_sorted, v0_sorted, v1_sorted,
				0.0f, 0.0f,
				shader, target_texture);
		}
		else
		{
			// Line that divides triangle into top and bottom
			line const separator_line{ v1_sorted.x, v1_sorted.y, v1_sorted.x + 1.0f, v1_sorted.y };
			// Calculate intersection point
			vector2f const intersection{ separator_line.intersection(line{v0_sorted.x, v0_sorted.y, v2_sorted.x, v2_sorted.y}) };
			// Distance in pixels from top vertex to intersection vertex and from top vertex to bottom vertex on the same edge
			// We need these because when we interpolate attributes across edge we need to know real edge length,
			// even though we draw top and bottom triangles separately
			//
			float const distance_to_separator_vertex{ vector2f{v0_sorted.x, v0_sorted.y}.distance_to(intersection) };
			float const total_edge_with_separator_vertex_length{ vector2f{v2_sorted.x, v2_sorted.y}.distance_to(vector2f{v0_sorted.x, v0_sorted.y}) };

			// Vertices that lie on separating line
			// We need two different vectors because we do not calculate interpolated values of z and w and use just distance offset
			vector4 separator_vertex_for_top_triangle{ intersection.x, intersection.y, v2.z, v2.w };
			vector4 separator_vertex_for_bottom_triangle{ intersection.x, intersection.y, v0.z, v0.w };

			// Draw top triangle
			rasterize_inversed_slope_top_or_bottom_triangle(
				index0_sorted, index2_sorted, index1_sorted,
				v0_sorted, separator_vertex_for_top_triangle, v1_sorted,
				total_edge_with_separator_vertex_length - distance_to_separator_vertex, 0.0f,
				shader, target_texture);
			// Draw bottom triangle
			rasterize_inversed_slope_top_or_bottom_triangle(
				index2_sorted, index0_sorted, index1_sorted,
				v2_sorted, separator_vertex_for_bottom_triangle, v1_sorted,
				distance_to_separator_vertex, 0.0f,
				shader, target_texture);
		}
	}

	template<typename TShader>
	void pipeline::rasterize_inversed_slope_top_or_bottom_triangle(
		unsigned int index0, unsigned int index1, unsigned int index2,
		vector4 v0, vector4 v1, vector4 v2,
		float v0_v1_edge_distance_offset, float v0_v2_edge_distance_offset,
		TShader& shader, texture& target_texture)
	{
		// Sort vertices on horizontal edge by their x-coordinate
		if (v1.x > v2.x)
		{
			std::swap(v1, v2);
			std::swap(index1, index2);
			std::swap(v0_v1_edge_distance_offset, v0_v2_edge_distance_offset);
		}
		// True = current triangle is a top triangle (v0.y < v1.y, v0.y < v2.y)
		bool const is_top_triangle{ v0.y < v1.y };
		// Direction coefficient
		int const y_order_coefficient{ is_top_triangle ? 1 : -1 };

		// Left edge length
		float const left_total_distance{ vector2f{v0.x, v0.y}.distance_to(vector2f{v1.x, v1.y}) + v0_v1_edge_distance_offset };
		// Right edge length
		float const right_total_distance{ vector2f{v0.x, v0.y}.distance_to(vector2f{v2.x, v2.y}) + v0_v2_edge_distance_offset };

		// For how many x's we move on the left edge for one y
		float const inversed_slope_left{ (v0.x - v1.x) / (v0.y - v1.y) * y_order_coefficient };
		// For how many x's we move on the right edge for one y
		float const inversed_slope_right{ (v0.x - v2.x) / (v0.y - v2.y) * y_order_coefficient };

		// Percent of left edge's length we pass when we increase y by one
		float const left_distance_step_normalized{ std::sqrt(1.0f + inversed_slope_left * inversed_slope_left) / left_total_distance };
		// Percent of right edge's length we pass when we increase y by one
		float const right_distance_step_normalized{ std::sqrt(1.0f + inversed_slope_right * inversed_slope_right) / right_total_distance };

		// Find next pixel center (if v0 is on the center already, move to the next pixel according to the top-left rule)
		float const next_y_pixel_center{ is_top_triangle ? get_next_pixel_center_exclusive(v0.y) : get_previous_pixel_center_exclusive(v0.y) };
		// Calculate offset from the top to the next pixel center
		float const next_y_pixel_center_delta{ std::abs(next_y_pixel_center - v0.y) };

		if ((next_y_pixel_center < 0.0f) || (next_y_pixel_center > target_texture.get_height()))
		{
			// If first scanline y coordinate is negative - we do not process a triangle
			// Because either it wasn't clipped or it does not occupy any pixel center (along y axis) in a texture
			return;
		}

		// Calculate last pixel y coordinate
		float const last_y_pixel_center{ is_top_triangle ? get_previous_pixel_center_exclusive(v1.y) : get_next_pixel_center_inclusive(v1.y) };
		if ((last_y_pixel_center < 0.0f) || (last_y_pixel_center > target_texture.get_height()))
		{
			// If first scanline y coordinate is negative - we do not process a triangle
			// Because either it wasn't clipped or it does not occupy any pixel center (along y axis) in a texture
			return;
		}

		// Calculate starting left distance considering offset to the next y pixel center
		float current_left_distance_normalized{ left_distance_step_normalized * next_y_pixel_center_delta };
		// Calculate starting right distance considering offset to the next y pixel center
		float current_right_distance_normalized{ right_distance_step_normalized * next_y_pixel_center_delta };

		// Calculate first scanline endpoints considering offset the next y pixel center
		float x_left{ v0.x + inversed_slope_left * next_y_pixel_center_delta };
		float x_right{ v0.x + inversed_slope_right * next_y_pixel_center_delta };
		// Calculate first and last scanlines integer y-coordinates
		int const first_y{ static_cast<int>(next_y_pixel_center) };
		int const last_y{ static_cast<int>(last_y_pixel_center) + y_order_coefficient };
		// Initialize attributes storage
		size_t color_binds_count{ m_binded_color_attributes.size() };
		std::vector<color> left_color_attributes(color_binds_count);
		std::vector<color> right_color_attributes(color_binds_count);

		size_t float_binds_count{ m_binded_float_attributes.size() };
		std::vector<float> left_float_attributes(float_binds_count);
		std::vector<float> right_float_attributes(float_binds_count);

		size_t vector2f_binds_count{ m_binded_vector2f_attributes.size() };
		std::vector<vector2f> left_vector2f_attributes(vector2f_binds_count);
		std::vector<vector2f> right_vector2f_attributes(vector2f_binds_count);

		size_t vector3f_binds_count{ m_binded_vector3f_attributes.size() };
		std::vector<vector3f> left_vector3f_attributes(vector3f_binds_count);
		std::vector<vector3f> right_vector3f_attributes(vector3f_binds_count);
		// For each scanline
		//
		for (int y{ first_y }; y != last_y; y += y_order_coefficient)
		{
			// Calculate attributes values on scanline endpoints
			//
			save_intermediate_attrs_values<color>(
				m_binded_color_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_color_attributes, right_color_attributes,
				v0.w, v1.w, v2.w);
			save_intermediate_attrs_values<float>(
				m_binded_float_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_float_attributes, right_float_attributes,
				v0.w, v1.w, v2.w);
			save_intermediate_attrs_values<vector2f>(
				m_binded_vector2f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector2f_attributes, right_vector2f_attributes,
				v0.w, v1.w, v2.w);
			save_intermediate_attrs_values<vector3f>(
				m_binded_vector3f_attributes,
				index0, index1, index2,
				current_left_distance_normalized, current_right_distance_normalized,
				left_vector3f_attributes, right_vector3f_attributes,
				v0.w, v1.w, v2.w);

			// Length of the scanline
			float const total_scanline_distance{ x_right - x_left };
			// Length we move for each x step
			float const scanline_step_distance_normalized{ 1.0f / total_scanline_distance };
			// Calculate first pixel center we start from, according to top-left rule

			float const x_left_next_pixel_center{ get_next_pixel_center_inclusive(x_left) };
			if (x_left_next_pixel_center < 0.0f)
			{
				continue;
			}
			// Calculate last pixel center
			float last_x_pixel_center{ get_previous_pixel_center_exclusive(x_right) };
			if (last_x_pixel_center < 0.0f)
			{
				continue;
			}
			// Calculate current scanline distance, taking into account starting left pixel offset
			float current_scanline_distance_normalized{ (x_left_next_pixel_center - x_left) * scanline_step_distance_normalized };

			int const first_x{ static_cast<int>(x_left_next_pixel_center) };
			int const last_x{ static_cast<int>(last_x_pixel_center) };

			// Calculate endpoints interpolated z reciprocals
			//
			float const left_zview_reciprocal = (1.0f - current_left_distance_normalized) * v0.w + current_left_distance_normalized * v1.w;
			float const right_zview_reciprocal = (1.0f - current_right_distance_normalized) * v0.w + current_right_distance_normalized * v2.w;

			for (int x{ first_x }; x <= last_x; ++x)
			{
				// Calculate attributes values on current pixel center
				//
				set_bind_points_values_from_scanline_endpoints<color>(
					m_binded_color_attributes,
					left_color_attributes, right_color_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);
				set_bind_points_values_from_scanline_endpoints<float>(
					m_binded_float_attributes,
					left_float_attributes, right_float_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);
				set_bind_points_values_from_scanline_endpoints<vector2f>(
					m_binded_vector2f_attributes,
					left_vector2f_attributes, right_vector2f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);
				set_bind_points_values_from_scanline_endpoints<vector3f>(
					m_binded_vector3f_attributes,
					left_vector3f_attributes, right_vector3f_attributes,
					current_scanline_distance_normalized,
					left_zview_reciprocal, right_zview_reciprocal);
				// Process pixel
				//
				vector2ui pixel{ static_cast<unsigned int>(x), static_cast<unsigned int>(y) };
				target_texture.set_pixel_color(pixel, shader.process_pixel(pixel));
				current_scanline_distance_normalized += scanline_step_distance_normalized;
			}
			// Move to the next scanline
			//
			x_left += inversed_slope_left;
			x_right += inversed_slope_right;
			current_left_distance_normalized += left_distance_step_normalized;
			current_right_distance_normalized += right_distance_step_normalized;
		}
	}

	template<typename TAttr>
	void pipeline::save_intermediate_attrs_values(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		unsigned int const top_vertex_index,
		unsigned int const left_vertex_index,
		unsigned int const right_vertex_index,
		float const distance_from_top_to_left_normalized,
		float const distance_from_top_to_right_normalized,
		std::vector<TAttr>& left_values_storage,
		std::vector<TAttr>& right_values_storage,
		float const top_vertex_zview_reciprocal,
		float const left_vertex_zview_reciprocal,
		float const right_vertex_zview_reciprocal)
	{
		size_t const binds_count{ binds.size() };
		for (size_t i{ 0 }; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];

			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();

			TAttr const& top_value = binded_attr_data[binded_attr_indices[top_vertex_index]];
			TAttr const& left_value = binded_attr_data[binded_attr_indices[left_vertex_index]];
			TAttr const& right_value = binded_attr_data[binded_attr_indices[right_vertex_index]];

			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				left_values_storage[i] = top_value * (1.0f - distance_from_top_to_left_normalized) + left_value * distance_from_top_to_left_normalized;
				right_values_storage[i] = top_value * (1.0f - distance_from_top_to_right_normalized) + right_value * distance_from_top_to_right_normalized;
			}
			else
			{
				TAttr const& top_value_div_zview = top_value * top_vertex_zview_reciprocal;
				TAttr const& left_value_div_zview = left_value * left_vertex_zview_reciprocal;
				TAttr const& right_value_div_zview = right_value * right_vertex_zview_reciprocal;
				left_values_storage[i] = top_value_div_zview * (1.0f - distance_from_top_to_left_normalized) + left_value_div_zview * distance_from_top_to_left_normalized;
				right_values_storage[i] = top_value_div_zview * (1.0f - distance_from_top_to_right_normalized) + right_value_div_zview * distance_from_top_to_right_normalized;
			}
		}
	}

	template<typename TAttr>
	void pipeline::set_bind_points_values_from_scanline_endpoints(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		std::vector<TAttr> const& left_endpoint_values,
		std::vector<TAttr> const& right_endpoint_values,
		float const scanline_distance_normalized,
		float const zview_reciprocal_left, float const zview_reciprocal_right)
	{
		size_t binds_count{ binds.size() };
		for (size_t i{ 0 }; i < binds_count; ++i)
		{
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			
			if (binded_attr.info.get_interpolation_option() == attribute_interpolation_option::linear)
			{
				TAttr const result =
					left_endpoint_values[i] * (1.0f - scanline_distance_normalized) +
					right_endpoint_values[i] * scanline_distance_normalized;
				(*binded_attr.bind_point) = result;
			}
			else
			{
				TAttr const value_div_zview_interpolated = left_endpoint_values[i] * (1.0f - scanline_distance_normalized) + right_endpoint_values[i] * scanline_distance_normalized;
				float const zview_reciprocal_interpolated = (1.0f - scanline_distance_normalized) * zview_reciprocal_left + scanline_distance_normalized * zview_reciprocal_right;

				(*binded_attr.bind_point) = value_div_zview_interpolated * (1.0f / zview_reciprocal_interpolated);
			}
		}
	}

	inline float pipeline::get_next_pixel_center_exclusive(float const f)
	{
		return (std::floor(f + 0.5f) + 0.5f);
	}

	inline float pipeline::get_next_pixel_center_inclusive(float const f)
	{
		return (std::floor(f + (0.5f - EPSILON)) + 0.5f);
	}

	inline float pipeline::get_previous_pixel_center_exclusive(float const f)
	{
		return (std::floor(f - (0.5f + EPSILON)) + 0.5f);
	}

	inline float clamp(float const value, float const from, float const to)
	{
		return (value > to ? to : (value < from ? from : value));
	}

	template<typename TShader>
	void pipeline::rasterize_homogeneous(
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		vector4 const& v0, vector4 const& v1, vector4 const& v2,
		TShader& shader, texture& target_texture)
	{
		// p = ax + by + cw
		// [p0 p1 p2] = [a b c] * m
		// [a b c] = [p0 p1 p2] * m_inversed
		matrix3x3 const m{
			v0.x, v1.x, v2.x,
			v0.y, v1.y, v2.y,
			v0.w, v1.w, v2.w };
		// Can also be interpreted as triple product
		float const det = m.det();

		// Check if m is invertible. If it's not, triangle shouldn't be rendered (volume == 0)
		//
		if (std::abs(det) < EPSILON)
		{
			return;
		}
		// Check if it's back facing
		//
		if (m_face_culling == face_culling_option::counter_clockwise)
		{
			if (det > 0)
			{
				return;
			}
		}
		else
		{
			if (det < 0)
			{
				return;
			}
		}
		matrix3x3 const m_inversed{ m.inversed_precalc_det(det) };
		// Edges pseudoparameters
		//
		vector3f const edge0_p{ 1.0f, 0.0f, 0.0f };
		vector3f const edge1_p{ 0.0f, 1.0f, 0.0f };
		vector3f const edge2_p{ 0.0f, 0.0f, 1.0f };
		// Edges coefficients
		//
		vector3f const edge0_abc{ m_inversed.values[0][0], m_inversed.values[0][1], m_inversed.values[0][2] };
		vector3f const edge1_abc{ m_inversed.values[1][0], m_inversed.values[1][1], m_inversed.values[1][2] };
		vector3f const edge2_abc{ m_inversed.values[2][0], m_inversed.values[2][1], m_inversed.values[2][2] };
		// 1/w function coefficients
		//
		vector3f const one_div_w_abc{ vector3f{1.0f, 1.0f, 1.0f} *m_inversed };
		// Calculate attributes coefficients
		//
		std::vector<vector3<color>> color_attrs_abc{ m_binded_color_attributes.size() };
		std::vector<vector3<float>> float_attrs_abc{ m_binded_float_attributes.size() };
		std::vector<vector3<vector2f>> vector2f_attrs_abc{ m_binded_vector2f_attributes.size() };
		std::vector<vector3<vector3f>> vector3f_attrs_abc{ m_binded_vector3f_attributes.size() };
		save_edges_coefficients<color>(
			m_binded_color_attributes,
			color_attrs_abc,
			index0, index1, index2,
			m_inversed);
		save_edges_coefficients<float>(
			m_binded_float_attributes,
			float_attrs_abc,
			index0, index1, index2,
			m_inversed);
		save_edges_coefficients<vector2f>(
			m_binded_vector2f_attributes,
			vector2f_attrs_abc,
			index0, index1, index2,
			m_inversed);
		save_edges_coefficients<vector3f>(
			m_binded_vector3f_attributes,
			vector3f_attrs_abc,
			index0, index1, index2,
			m_inversed);
		// Calculate bounding box
		//
		aabb<vector2ui> bounding_box;
		if ((v0.w < EPSILON) || (v1.w < EPSILON) || (v2.w < EPSILON))
		{
			bounding_box.from = vector2ui{ 0, 0 };
			bounding_box.to = vector2ui{
				static_cast<unsigned int>(target_texture.get_width()) - 1,
				static_cast<unsigned int>(target_texture.get_height()) - 1 };
		}
		else
		{
			float v0_screen_x{ v0.x / v0.w };
			v0_screen_x = clamp(v0_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));
			float v0_screen_y{ v0.y / v0.w };
			v0_screen_y = clamp(v0_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));
			float v1_screen_x{ v1.x / v1.w };
			v1_screen_x = clamp(v1_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));
			float v1_screen_y{ v1.y / v1.w };
			v1_screen_y = clamp(v1_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));
			float v2_screen_x{ v2.x / v2.w };
			v2_screen_x = clamp(v2_screen_x, 0.0f, static_cast<float>(target_texture.get_width()));
			float v2_screen_y{ v2.y / v2.w };
			v2_screen_y = clamp(v2_screen_y, 0.0f, static_cast<float>(target_texture.get_height()));
			bounding_box.from.x = static_cast<unsigned int>(std::min(std::min(v0_screen_x, v1_screen_x), v2_screen_x));
			bounding_box.from.y = static_cast<unsigned int>(std::min(std::min(v0_screen_y, v1_screen_y), v2_screen_y));
			bounding_box.to.x = static_cast<unsigned int>(std::max(std::max(v0_screen_x, v1_screen_x), v2_screen_x));
			bounding_box.to.y = static_cast<unsigned int>(std::max(std::max(v0_screen_y, v1_screen_y), v2_screen_y));
		}
		for (unsigned int y{ bounding_box.from.y }; y <= bounding_box.to.y; ++y)
		{
			vector2f const first_pixel_center{ bounding_box.from.x + 0.5f, y + 0.5f };
			float edge0_v{ edge0_abc.x * first_pixel_center.x + edge0_abc.y * first_pixel_center.y + edge0_abc.z };
			float edge1_v{ edge1_abc.x * first_pixel_center.x + edge1_abc.y * first_pixel_center.y + edge1_abc.z };
			float edge2_v{ edge2_abc.x * first_pixel_center.x + edge2_abc.y * first_pixel_center.y + edge2_abc.z };
			float one_div_w_v{ one_div_w_abc.x * first_pixel_center.x + one_div_w_abc.y * first_pixel_center.y + one_div_w_abc.z };
			for (unsigned int x{ bounding_box.from.x }; x <= bounding_box.to.x; ++x)
			{
				vector2ui const p{ x, y };
				vector2f const pc{ x + 0.5f, y + 0.5f };
				// Because we're interested only in sign of p, and not the actual value, we can multiply instead of dividing, sign will be the same
				//
				if (is_point_on_positive_halfspace_top_left(edge0_v * one_div_w_v, edge0_abc.x, edge0_abc.y) &&
					is_point_on_positive_halfspace_top_left(edge1_v * one_div_w_v, edge1_abc.x, edge1_abc.y) &&
					is_point_on_positive_halfspace_top_left(edge2_v * one_div_w_v, edge2_abc.x, edge2_abc.y))
				{
					// Check if point is behind
					//
					if (one_div_w_v < 0.0f)
					{
						continue;
					}
					float const w_value{ 1.0f / one_div_w_v };
					set_bind_points_values_from_edge_coefficients<color>(
						m_binded_color_attributes,
						color_attrs_abc,
						pc,
						w_value);
					set_bind_points_values_from_edge_coefficients<float>(
						m_binded_float_attributes,
						float_attrs_abc,
						pc,
						w_value);
					set_bind_points_values_from_edge_coefficients<vector2f>(
						m_binded_vector2f_attributes,
						vector2f_attrs_abc,
						pc,
						w_value);
					set_bind_points_values_from_edge_coefficients<vector3f>(
						m_binded_vector3f_attributes,
						vector3f_attrs_abc,
						pc,
						w_value);
					target_texture.set_pixel_color(p, shader.process_pixel(p));
				}
				edge0_v += edge0_abc.x;
				edge1_v += edge1_abc.x;
				edge2_v += edge2_abc.x;
				one_div_w_v += one_div_w_abc.x;
			}
		}
	}

	template<typename TAttr>
	void pipeline::save_edges_coefficients(
		std::vector<binded_mesh_attribute_info<TAttr>> const& binds,
		std::vector<vector3<TAttr>>& coefficients_storage,
		unsigned int const index0, unsigned int const index1, unsigned int const index2,
		matrix3x3 const& vertices_matrix_inversed)
	{
		for (size_t i = 0; i < binds.size(); ++i)
		{
			binded_mesh_attribute_info<TAttr> const& binded_attr = binds[i];
			std::vector<TAttr> const& binded_attr_data = binded_attr.info.get_data();
			std::vector<unsigned int> const& binded_attr_indices = binded_attr.info.get_indices();
			TAttr const& value0 = binded_attr_data[binded_attr_indices[index0]];
			TAttr const& value1 = binded_attr_data[binded_attr_indices[index1]];
			TAttr const& value2 = binded_attr_data[binded_attr_indices[index2]];
			coefficients_storage[i] = vector3<TAttr>{ value0, value1, value2 } *vertices_matrix_inversed;
		}
	}

	template<typename TAttr>
	void pipeline::set_bind_points_values_from_edge_coefficients(
		std::vector<binded_mesh_attribute_info<TAttr>>& binds,
		std::vector<vector3<TAttr>> const& coefficients_storage,
		vector2f const& point,
		float const w)
	{
		for (size_t i = 0; i < binds.size(); ++i)
		{
			vector3<TAttr> abc{ coefficients_storage.at(i) };
			TAttr const value_div_w = abc.x * point.x + abc.y * point.y + abc.z;
			binded_mesh_attribute_info<TAttr>& binded_attr = binds[i];
			(*binded_attr.bind_point) = value_div_w * w;
		}
	}
}
#endif //!DAISY_PIPELINE_H