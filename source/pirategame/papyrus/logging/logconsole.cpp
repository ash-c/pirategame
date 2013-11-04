
// Library Includes

// Local Includes
#include "logconsole.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Logger::CLogConsole::CLogConsole()
	: m_font(0)
	, m_surface(0)
	, m_texture(0)
	, m_active(false)
{
	m_col.r = 255;
	m_col.g = 255;
	m_col.b = 255;
	m_col.a = 255;
	
	m_pos.x = 0;
	m_pos.y = 0;
	m_pos.w = 0;
	m_pos.h = 0;

	m_textPos.x = SM_DEFAULT_X;
	m_textPos.w = 0;
	m_textPos.h = 0;

	SDL_memset(m_displayText, 0, sizeof(Int8*) * SM_MAXDISPLAY);
}

Logger::CLogConsole::~CLogConsole()
{
}

Bool Logger::CLogConsole::Initialise(const Int8* _path)
{
	m_font = TTF_OpenFont("data/fonts/Oxygen-Regular.ttf", 18);
	if (nullptr == m_font)
	{
		return false;
	}

	return true;
}

Bool Logger::CLogConsole::ShutDown()
{
	if (0 != m_surface)
	{
		SDL_FreeSurface(m_surface);
	}
	TTF_CloseFont(m_font);
	
	for (Int16 i = 0; i < SM_MAXDISPLAY; ++i)
	{
		CLEANARRAY(m_displayText[i]);
	}

	return true;
}

void Logger::CLogConsole::Process(Float32 _fDelta)
{
	if (m_active)
	{
		m_surface = SDL_CreateRGBSurface(0, 1600, 450, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		SDL_FillRect(m_surface, NULL, 0x00000088); // Transparent black

		SDL_Surface* text = 0;
		m_textPos.y = 450 - 50;
		for (Int16 i = (SM_MAXDISPLAY - 1); i >= 0; --i)
		{
			if (0 != m_displayText[i])
			{
				text = TTF_RenderText_Blended(m_font, m_displayText[i], m_col);
				SDL_BlitSurface(text, NULL, m_surface, &m_textPos);
				SDL_FreeSurface(text);
				m_textPos.y -= 20;
			}
		}

		Renderer::activeRenderer->LoadTexture(m_surface, &m_texture);
		SDL_FreeSurface(m_surface);
		m_surface = 0;
		assert(m_texture);
		SDL_QueryTexture(m_texture, NULL, NULL, &m_pos.w, &m_pos.h);
	}
}

void Logger::CLogConsole::Render()
{
	if (m_active)
	{
		if (m_texture)
		{
			Renderer::activeRenderer->Render(m_texture, &m_pos, NULL);
			SDL_DestroyTexture(m_texture);
			m_texture = 0;
		}
	}
}

void Logger::CLogConsole::Write(Int8* _msg)
{
	for (Int16 i = 0; i < SM_MAXDISPLAY; ++i)
	{
		if (0 == m_displayText[i])
		{
			m_displayText[i] = new Int8[MAX_BUFFER];
			SDL_strlcpy(m_displayText[i], _msg, MAX_BUFFER);
			return; // Message added, leave function.
		}
	}

	// If we get this far, there is nowhere to add the new message.
	// Delete the first one, and add the new one to the end.
	CLEANARRAY(m_displayText[0]);

	UInt16 i;
	for (i = 0; i < (SM_MAXDISPLAY - 1); ++i)
	{
		m_displayText[i] = m_displayText[i + 1];
	}

	m_displayText[i] = new Int8[MAX_BUFFER];
	SDL_strlcpy(m_displayText[i], _msg, MAX_BUFFER);
}

Bool Logger::CLogConsole::Toggle()
{
	m_active = !m_active;
	return m_active;
}