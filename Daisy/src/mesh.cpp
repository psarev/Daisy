#include "../include/mesh.h"

using namespace daisy;

mesh::mesh(std::vector<vector3> const& vertices, std::vector<face> const& faces) :
	m_vertices{ vertices }, m_faces{ faces }
{
}

std::vector<vector3> const& mesh::get_vertices() const
{
	return m_vertices;
}

std::vector<face> const& mesh::get_faces() const
{
	return m_faces;
}