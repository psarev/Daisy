#ifndef DAISY_APP_H
#define DAISY_APP_H

#include "SDL.h"
#include "pipeline.h"

namespace daisy {
	class app
	{
	public:
		app(unsigned int const width, unsigned int const height);
		virtual ~app();

		int start();
	
	protected:
		texture& get_target_texture();

		pipeline& get_pipeline();

		void set_target_framerate(unsigned int const fps);

		virtual void update(float const delta_time) = 0;
		virtual void on_key_down(SDL_Keysym const key);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_sdl_target_texture;

		texture m_target_texture;
		pipeline m_pipeline;
		Uint32 m_target_framerate_delay;
	};
}

#endif // !DAISY_APP_H
