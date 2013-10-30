
// Library Includes

// Local Includes
#include "logtoscreen.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Logger::CLogToScreen::CLogToScreen()
	: m_font(0)
	, m_surface(0)
	, m_texture(0)
	, m_trackedText(0)
	, m_displayText(0)
{
	m_col.r = 255;
	m_col.g = 255;
	m_col.b = 255;
	m_col.a = 0;

	m_pos.x = 10;
	m_pos.y = 10;
}

Logger::CLogToScreen::~CLogToScreen()
{
}

Bool Logger::CLogToScreen::Initialise()
{
	if (TTF_Init() != 0)
	{
		return false;
	}

	m_font = TTF_OpenFont("data/fonts/Oxygen-Regular.ttf", 18);
	if (nullptr == m_font)
	{
		return false;
	}

	m_surface = SDL_CreateRGBSurface(0, 800, 600, 32, 0, 0, 0, 0);

	m_trackedText = new Int8[SM_MAXTRACKED];
	m_displayText= new Int8[SM_MAXWRITE];

	return true;
}

Bool Logger::CLogToScreen::ShutDown()
{
	CLEANARRAY(m_trackedText);
	CLEANARRAY(m_displayText);
	SDL_FreeSurface(m_surface);
	TTF_CloseFont(m_font);
	return true;
}

void Logger::CLogToScreen::Process(Float32 _fDelta)
{
	SDL_Rect dest = {m_pos.x, m_pos.y, 0, 0};

	SDL_Surface* text = TTF_RenderText_Blended(m_font, "line1", m_col);
	SDL_BlitSurface(text, NULL, m_surface, &dest);
	SDL_FreeSurface(text);

	dest.y += 20;

	text = TTF_RenderText_Blended(m_font, "line2", m_col);
	SDL_BlitSurface(text, NULL, m_surface, &dest);
	SDL_FreeSurface(text);

	//m_surface = TTF_RenderText_Blended(m_font, _msg, m_col);

	Renderer::activeRenderer->LoadTexture(m_surface, &m_texture);
	assert(m_texture);
	SDL_QueryTexture(m_texture, NULL, NULL, &m_pos.w, &m_pos.h);
}

void Logger::CLogToScreen::Render()
{
	Renderer::activeRenderer->Render(m_texture, &m_pos, NULL);
}

void Logger::CLogToScreen::Write(Int8* _msg)
{
}