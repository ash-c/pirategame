
// Library Includes
#include <SDL_image.h>

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
	assert(_width >= 800 && _height >= 600 && "Resolution too low.");
	m_Height = _height;
	m_Width = _width;
	m_FullScreen = _fullScreen;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return false;
	}
	
	UInt32 flags = 0;
	if (m_FullScreen)
	{
		flags = SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_BORDERLESS;
	}
	/*else
	{
		flags = SDL_WINDOW_BORDERLESS;
	}*/

	m_Window = SDL_CreateWindow(_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, flags);
	if (nullptr == m_Window)
	{
		return false;
	}

	SDL_Surface* icon = IMG_Load("data/icon.png");
	SDL_SetWindowIcon(m_Window, icon);
	SDL_FreeSurface(icon);

	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
	if (nullptr == m_Renderer)
	{
		return false;
	}

	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 0);

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

void Renderer::CSDLRenderer::Render(SDL_Texture* _tex, SDL_Rect* _dst, SDL_Rect* _clip)
{
	SDL_RenderCopy(m_Renderer, _tex, _clip, _dst);
}

void Renderer::CSDLRenderer::Present()
{
	SDL_RenderPresent(m_Renderer);
}

Bool Renderer::CSDLRenderer::LoadTexture(Int8* _path, SDL_Texture** _result)
{
	*_result = IMG_LoadTexture(m_Renderer, _path);
	assert(*_result);
	return true;
}

Bool Renderer::CSDLRenderer::LoadTexture(SDL_Surface* _surface, SDL_Texture** _result)
{
	*_result = SDL_CreateTextureFromSurface(m_Renderer, _surface);
	assert(*_result);
	return true;
}