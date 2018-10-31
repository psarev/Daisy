#ifndef DAISY_APP_H
#define DAISY_APP_H

#include <SDL.h>
#include "bitmap_painter.h"

namespace daisy {
	class app
	{
	public:
		app(unsigned int const width, unsigned int const height);
		virtual ~app();

		int start();
	
	protected:
		bitmap_painter& get_painter();

		virtual void update(float delta_time) = 0;
		virtual void on_key_down(SDL_Keysym key);

	private:
		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_target_texture;
		bitmap_painter m_painter;
	};
}

#endif // !DAISY_APP_H
