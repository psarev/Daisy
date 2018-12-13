#include <iostream>
#include <stdexcept>
#include <SDL_image.h>
#include "app.h"

using namespace daisy;

const char* APP_NAME = "Daisy";
const char* APP_VERSION = "0.02";

app::app(unsigned int const width, unsigned int const height) :
	m_window{ nullptr },
	m_renderer{ nullptr },
	m_sdl_target_texture{ nullptr },
	m_target_texture{ width, height },
	m_target_framerate_delay(0)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error(SDL_GetError());
	}

	char title[256];
	title[255] = '\0';
	snprintf(title, 255,
		"%s %s",
		APP_NAME, APP_VERSION);

	int flags = IMG_INIT_PNG;
	int inited = IMG_Init(flags);
	if ((inited & flags) != flags)
	{
		throw std::runtime_error(IMG_GetError());
	}

	m_window = SDL_CreateWindow(
		title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height,
		SDL_WINDOW_SHOWN);

	if (m_window == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	m_sdl_target_texture = SDL_CreateTexture(
		m_renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		width, height);

	if (m_sdl_target_texture == nullptr)
	{
		throw std::runtime_error(SDL_GetError());
	}

	set_target_framerate(60);
}

app::~app()
{
	if (m_sdl_target_texture != nullptr)
	{
		SDL_DestroyTexture(m_sdl_target_texture);
		m_sdl_target_texture = nullptr;
	}

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
		m_window = nullptr;
	}
	
	IMG_Quit();

	SDL_Quit();
}

int app::start()
{
	bool running(true);
	SDL_Event event;

	Uint32 last_frame_time{ 0 };

	Uint32 time_accumulator{ 0 };
	unsigned int frames_accumulator{ 0 };

	while (running)
	{
		Uint32 current_Time{ SDL_GetTicks() };
		Uint32 delta_time{ current_Time - last_frame_time };

		last_frame_time = SDL_GetTicks();

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				on_key_down(event.key.keysym);
			}
		}

		m_target_texture.clear(0);

		update(delta_time / 1000.0f);

		time_accumulator += delta_time;

		SDL_UpdateTexture(m_sdl_target_texture, nullptr, m_target_texture.get_data(), m_target_texture.get_pitch());
		SDL_RenderCopy(m_renderer, m_sdl_target_texture, nullptr, nullptr);
		SDL_RenderPresent(m_renderer);

		++frames_accumulator;

		if (m_target_framerate_delay > 0)
		{
			Uint32 time_required_for_frame = SDL_GetTicks() - last_frame_time;
			int time_to_wait = m_target_framerate_delay - time_required_for_frame;

			if (time_to_wait > 0)
			{
				SDL_Delay(static_cast<Uint32>(time_to_wait));
			}
		}

		if (time_accumulator >= 1000)
		{

#define DAISY_OUTPUT_FPS
#ifdef DAISY_OUTPUT_FPS
			//std::cout << "FPS: " << frames_accumulator << std::endl;
			char title[256];
			title[255] = '\0';
			snprintf(title, 255,
				"%s %s - [FPS: %d]",
				APP_NAME, APP_VERSION, frames_accumulator);
			SDL_SetWindowTitle(m_window, title);
#endif
			time_accumulator = 0;
			frames_accumulator = 0;
		}
	}

	return 0;
}

texture& app::get_target_texture()
{
	return m_target_texture;
}

pipeline& app::get_pipeline()
{
	return m_pipeline;
}

void app::on_key_down(SDL_Keysym const)
{

}

void app::set_target_framerate(unsigned int const fps)
{
	if (fps == 0)
	{
		m_target_framerate_delay = 0;
	}
	else
	{
		m_target_framerate_delay = 1000 / fps;
	}
}

