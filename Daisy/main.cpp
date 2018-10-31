#include "include/app.h"
#include "include/scene.h"
#include "include/obj_import.h"
#include "include/pipeline.h"

using namespace daisy;

class default_app : public app
{
public:
	default_app(unsigned int const width, unsigned int const height);

protected:
	void update(float delta_time) override;
	void on_key_down(SDL_Keysym key) override;

private:
	scene m_scene;
	camera m_camera;
	pipeline m_pipeline;
};

default_app::default_app(unsigned int const width, unsigned int const height) :
	app(width, height),
	m_camera{
		vector3::ZERO,
		vector3(0.0f, 0.0f, 1.0f),
		vector3::Y_UNIT,
		static_cast<float>(M_PI) / 2.0f,
		static_cast<float>(height) / static_cast<float>(width),
		0.01f,
		20.0f }
{
	scene_object default_object{ import_obj_mesh("resources/bunny.obj") };
	default_object.set_world_position(vector3{ 0.0f, 0.0f, 1.0f });
	//default_object.set_world_rotation(vector3{ static_cast<float>(M_PI) / 8.0f, static_cast<float>(M_PI), 0.0f });
	m_scene.get_objects().push_back(default_object);
}

void default_app::update(float delta_time) {
	//m_scene.get_objects()[0].get_world_rotation().y += static_cast<float>(M_PI) / 4.0f * delta_time;

	m_pipeline.draw_scene(m_scene, m_camera, get_painter());
}

void default_app::on_key_down(SDL_Keysym key)
{
	app::on_key_down(key);

	float const moving_speed = 0.1f;
	float const rotation_speed = 0.05f;

	if (key.sym == SDLK_a)
	{
		m_camera.move_left(moving_speed);
	} else if (key.sym == SDLK_d)
	{
		m_camera.move_right(moving_speed);
	}

	if (key.sym == SDLK_w)
	{
		m_camera.move_forward(moving_speed);
	}
	else if (key.sym == SDLK_s)
	{
		m_camera.move_backward(moving_speed);
	}

	if (key.sym == SDLK_r)
	{
		m_camera.move_up(moving_speed);
	}
	else if (key.sym == SDLK_f)
	{
		m_camera.move_down(moving_speed);
	}

	if (key.sym == SDLK_q)
	{
		//m_camera.yaw(-rotation_speed);
		m_scene.get_objects()[0].get_world_rotation().y -= static_cast<float>(M_PI) / 4.0f;
	}
	else if (key.sym == SDLK_e)
	{
		//m_camera.yaw(rotation_speed);
		m_scene.get_objects()[0].get_world_rotation().y += static_cast<float>(M_PI) / 4.0f;
	}

}

int main(int argc, char* argv[])
{

	return default_app{ 800, 600 }.start();

}
