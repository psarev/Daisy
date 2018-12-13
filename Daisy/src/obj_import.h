#ifndef DAISY_OBJ_IMPORT_H
#define DAISY_OBJ_IMPORT_H

#include <string>
#include "mesh.h"

namespace daisy {
	class obj_reader
	{
	public:
		void read(std::string const& path);

		typedef void(obj_reader::*parse_face_function)(std::istringstream&);

	protected:
		virtual void on_reading_started();
		virtual void on_reading_ended();

		virtual void on_vertex_def(float const x, float const y, float const z);
		virtual void on_texcoord_def(float const x, float const y);
		virtual void on_normal_def(float const x, float const y, float const z);

		virtual void on_face_def_started();
		virtual void on_face_def_ended();
		virtual void on_face_pos_def(
			unsigned int vertex_index0,
			unsigned int vertex_index1,
			unsigned int vertex_index2);
		virtual void on_face_texcoord_def(
			unsigned int texcoord_index0,
			unsigned int texcoord_index1,
			unsigned int texcoord_index2);
		virtual void on_face_normal_def(
			unsigned int normal_index0,
			unsigned int normal_index1,
			unsigned int normal_index2);

	private:
		parse_face_function get_parse_function(std::string face_def);

		void parse_face_vertex(std::istringstream&);
		void parse_face_vertex_normal(std::istringstream&);
		void parse_face_vertex_texcoord(std::istringstream&);
		void parse_face_vertex_texcoord_normal(std::istringstream&);
	};

	class obj_mesh_importer final : public obj_reader
	{
	public:
		obj_mesh_importer(bool const read_texcoords, bool const read_normals);

		mesh get_mesh() const;

		virtual void on_reading_started() override;

		virtual void on_vertex_def(float const x, float const y, float const z) override;
		virtual void on_texcoord_def(float const x, float const y) override;
		virtual void on_normal_def(float const x, float const y, float const z) override;

		virtual void on_face_pos_def(
			unsigned int vertex_index0,
			unsigned int vertex_index1,
			unsigned int vertex_index2) override;
		virtual void on_face_texcoord_def(
			unsigned int texcoord_index0,
			unsigned int texcoord_index1,
			unsigned int texcoord_index2) override;
		virtual void on_face_normal_def(
			unsigned int normal_index0,
			unsigned int normal_index1,
			unsigned int normal_index2) override;

	private:
		bool const m_read_texcoords;
		bool const m_read_normals;

		std::vector<vector3f> m_vertices;
		std::vector<unsigned int> m_indices;

		std::vector<vector2f> m_texcoords;
		std::vector<unsigned int> m_texcoords_indices;

		std::vector<vector3f> m_normals;
		std::vector<unsigned int> m_normals_indices;
	};

	mesh load_mesh_from_obj(std::string const& path, bool const read_texcoords, bool const read_normals);
}

#endif // !DAISY_OBJ_IMPORT_H
