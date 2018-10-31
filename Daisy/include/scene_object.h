#ifndef DAISY_SCENE_OBJECT_H
#define DAISY_SCENE_OBJECT_H

#include <memory>
#include "mesh.h"

namespace daisy {
	class scene_object final
	{
	public:
		scene_object(std::shared_ptr<mesh const> mesh);

		std::shared_ptr<mesh const> get_mesh() const;
		vector3 get_world_position() const;
		vector3 get_world_rotation() const;
		vector3& get_world_rotation();

		void set_world_position(vector3 const& position);
		void set_world_rotation(vector3 const& rotation);

	private:
		std::shared_ptr<mesh const> m_mesh;
		vector3 m_world_position;
		vector3 m_world_rotation;
	};
}

#endif // !DAISY_SCENE_OBJECT_H
