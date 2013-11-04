
// Library Includes

// Local Includes
#include "logtoscreen.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Logger::CLogToScreen::CLogToScreen()
	: m_font(0)
	, m_surface(0)
	, m_texture(0)
	, m_timer(0.0f)
	, m_active(true)
{
	m_col.r = 255;
	m_col.g = 255;
	m_col.b = 255;
	m_col.a = 0;

	m_pos.w = 0;
	m_pos.h = 0;

	SDL_memset(m_tempText, 0, sizeof(Int8*) * SM_MAXTEMP);
	SDL_memset(m_trackedInfo, 0, sizeof(TTrackInfo*) * SM_MAXTRACKED);
}

Logger::CLogToScreen::~CLogToScreen()
{
}

Bool Logger::CLogToScreen::Initialise(const Int8* _path)
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

	return true;
}

Bool Logger::CLogToScreen::ShutDown()
{
	for (UInt16 i = 0; i < SM_MAXTEMP; ++i)
	{
		CLEANARRAY(m_tempText[i]);
	}

	for (UInt16 i = 0; i < SM_MAXTRACKED; ++i)
	{
		if (0 != m_trackedInfo[i])
		{
			CLEANARRAY(m_trackedInfo[i]->tag);
			CLEANDELETE(m_trackedInfo[i]);
		}
	}

	if (0 != m_surface)
	{
		SDL_FreeSurface(m_surface);
	}
	TTF_CloseFont(m_font);
	return true;
}

void Logger::CLogToScreen::Process(Float32 _fDelta)
{
	if (m_active)
	{
		m_surface = SDL_CreateRGBSurface(0, 400, 600, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		Bool messageAdded = false;

		// Set up surface for display
		SDL_Surface* text = 0;
		// Tracked text first.
		for (UInt16 i = 0; i < SM_MAXTRACKED; ++i)
		{
			if (0 != m_trackedInfo[i])
			{
				Int8 displayText[MAX_BUFFER];
				switch (m_trackedInfo[i]->type)
				{
				case TRACKTYPE_BOOL:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(Bool*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_CHAR:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(Int8*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_INT:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(Int32*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_UINT:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(UInt32*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_FLOAT:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(Float32*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_VECTOR3:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(VECTOR3*)(m_trackedInfo[i]->value));
					break;
				case TRACKTYPE_VECTOR4:
					SDL_snprintf(displayText, MAX_BUFFER, "%s: %.2f", m_trackedInfo[i]->tag, *(VECTOR4*)(m_trackedInfo[i]->value));
					break;
				default:
					break;
				}
			
				text = TTF_RenderText_Blended(m_font, displayText, m_col);
				SDL_BlitSurface(text, NULL, m_surface, &m_pos);
				SDL_FreeSurface(text);
				messageAdded = true;
				m_pos.y += 20;
			}
		}
	
		// Then temporary messages.
		for (UInt16 i = 0; i < SM_MAXTEMP; ++i)
		{
			if (0 != m_tempText[i])
			{
				text = TTF_RenderText_Blended(m_font, m_tempText[i], m_col);
				SDL_BlitSurface(text, NULL, m_surface, &m_pos);
				SDL_FreeSurface(text);
				messageAdded = true;
				m_pos.y += 20;
			}
		}
	
		if (messageAdded)
		{
			Renderer::activeRenderer->LoadTexture(m_surface, &m_texture);
			SDL_FreeSurface(m_surface);
			m_surface = 0;
			assert(m_texture);
			SDL_QueryTexture(m_texture, NULL, NULL, &m_pos.w, &m_pos.h);
		}

		// Remove the first temporary message in the list that has been here for more than SM_TIMER seconds.
		m_timer += _fDelta;
		if (m_timer > 0.0f && m_timer > SM_TIMER)
		{
			if (0 != m_tempText[0])
			{
				CLEANARRAY(m_tempText[0]);
				m_timer = 0.0f;

				for (UInt16 i = 0; i < (SM_MAXTEMP - 1); ++i)
				{
					m_tempText[i] = m_tempText[i + 1];
					if (0 == m_tempText[i]) break;
				}
			}
		}
	}
}

void Logger::CLogToScreen::Render()
{
	if (m_active)
	{
		m_pos.x = SM_DEFAULT_X;
		m_pos.y = SM_DEFAULT_Y;
		if (m_texture)
		{
			Renderer::activeRenderer->Render(m_texture, &m_pos, NULL);
			SDL_DestroyTexture(m_texture);
			m_texture = 0;
		}
	}
}

void Logger::CLogToScreen::Write(Int8* _msg)
{
	if (m_active)
	{
		for (UInt16 i = 0; i < SM_MAXTEMP; ++i)
		{
			if (0 == m_tempText[i])
			{			
				m_tempText[i] = new Int8[MAX_BUFFER];
				SDL_strlcpy(m_tempText[i], _msg, MAX_BUFFER);
				break;
			}
		}
	}
}

Bool Logger::CLogToScreen::Toggle()
{
	m_active = !m_active;
	return m_active;
}

void Logger::CLogToScreen::TrackValue(void* _value, ETrackType _type, const Int8* _tag)
{
	for (UInt16 i = 0; i < SM_MAXTRACKED; ++i)
	{
		if (0 == m_trackedInfo[i])
		{
			m_trackedInfo[i] = new TTrackInfo();
			m_trackedInfo[i]->type = _type;
			m_trackedInfo[i]->value = (_value);
			m_trackedInfo[i]->tag = new Int8[MAX_BUFFER];
			SDL_strlcpy(m_trackedInfo[i]->tag, _tag, MAX_BUFFER);
			break;
		}
	}
}

void Logger::CLogToScreen::StopTracking(const Int8* _tag)
{
	for (UInt16 i = 0; i < SM_MAXTRACKED; ++i)
	{
		if (0 != m_trackedInfo[i])
		{
			if (!SDL_strcmp(_tag, m_trackedInfo[i]->tag))
			{
				CLEANARRAY(m_trackedInfo[i]->tag);
				CLEANDELETE(m_trackedInfo[i]);
				break;
			}
		}
	}
}