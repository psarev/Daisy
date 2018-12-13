#ifndef DAISY_TRIANGLE_APP_H
#define DAISY_TRIANGLE_APP_H

#include "app.h"
#include "obj_import.h"
#include "camera.h"
#include "color_shader.h"
#include "texture_shader.h"

namespace daisy {

	enum class shader_option
	{
		color,
		texture
	};

	class triangle_app : public app
	{
	public:
		triangle_app(unsigned int const width, unsigned int const height);

	protected:
		void update(float const delta_time) override;
		void on_key_down(SDL_Keysym const key) override;

	private:
		void update_shader_mvp();

		vector3f const m_triangle_position;
		vector3f m_triangle_rotation;
		mesh m_triangle_mesh;

		camera m_camera;

		color_shader m_color_shader;
		texture_shader m_texture_shader;
		shader_option m_shader_option;

		texture m_texture;
	};
}

#endif // !DAISY_TRIANGLE_APP_H
