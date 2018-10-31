#ifndef DAISY_PIPELINE_H
#define DAISY_PIPELINE_H

#include <memory>
#include "scene.h"
#include "camera.h"
#include "bitmap_painter.h"

namespace daisy {
	class pipeline final
	{
	public:
		void draw_scene(scene const& scene, camera const& camera, bitmap_painter& painter) const;

	private:
		void draw_mesh(
			std::shared_ptr<mesh const> mesh,
			vector3 const& position,
			vector3 const& rotation,
			camera const& camera,
			bitmap_painter& painter) const;
	};
}

#endif // !DAISY_PIPELINE_H
