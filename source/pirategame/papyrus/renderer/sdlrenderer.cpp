
// Library Includes

// Local Includes
#include "sdlrenderer.h"

using namespace Papyrus;

Renderer::CSDLRenderer::CSDLRenderer()
{
}

Renderer::CSDLRenderer::~CSDLRenderer()
{
}

Bool Renderer::CSDLRenderer::Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen)
{
	assert(_width >= 1280 && _height >= 1024 && "Resolution too low.");
	m_Height = _height;
	m_Width = _width;
	m_FullScreen = _fullScreen;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return false;
	}
	
	m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS);
	if (nullptr == m_Window)
	{
		return false;
	}

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (nullptr == m_Renderer)
	{
		return false;
	}

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 0);

	return true;
}

Bool Renderer::CSDLRenderer::ShutDown()
{
	SDL_DestroyRenderer(m_Renderer);
	SDL_DestroyWindow(m_Window);

	return true;
}

void Renderer::CSDLRenderer::Clear()
{
	SDL_RenderClear(m_Renderer);
}

void Renderer::CSDLRenderer::Present()
{
	SDL_RenderPresent(m_Renderer);
}