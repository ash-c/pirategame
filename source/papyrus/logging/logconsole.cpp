
// Library Includes

// Local Includes
#include "logconsole.h"
#include "logger.h"
#include "../renderer/renderer.h"

using namespace Papyrus;

Logger::CLogConsole::CLogConsole()
	: m_font(0)
	, m_surface(0)
	, m_texture(0)
	, m_height(0)
	, m_width(0)
	, m_active(false)
	, m_registered(false)
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
	m_font = TTF_OpenFont("data/papyrus/fonts/Oxygen-Regular.ttf", SM_FONTSIZE);
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
	// Registering here as input isn't created yet when the console is initialised
	if (!m_registered) 
	{
		Input::inputManager->Register(this);
		m_registered = true;
	}

	if (m_active)
	{
		if (0 == m_height && 0 == m_width)
		{
			m_width = Renderer::activeRenderer->GetWidth();
			m_height = Renderer::activeRenderer->GetHeight()/2;
		}

		m_surface = SDL_CreateRGBSurface(0, m_width, m_height, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
		SDL_FillRect(m_surface, NULL, 0x00000088); // Transparent black

		SDL_Surface* text = 0;
		m_textPos.y = static_cast<Int32>(m_height - (SM_FONTSIZE * 1.5));

		//if (!m_input.isEmpty())
		//{
			text = TTF_RenderText_Blended(m_font, m_input.render(), m_col);
			SDL_BlitSurface(text, NULL, m_surface, &m_textPos);
			SDL_FreeSurface(text);
		//}

		m_textPos.y -= static_cast<Int32>(SM_FONTSIZE * 1.5);

		for (Int16 i = (SM_MAXDISPLAY - 1); i >= 0; --i)
		{
			if (0 != m_displayText[i])
			{
				text = TTF_RenderText_Blended(m_font, m_displayText[i], m_col);
				SDL_BlitSurface(text, NULL, m_surface, &m_textPos);
				SDL_FreeSurface(text);
				m_textPos.y -= (SM_FONTSIZE + 2);
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

void Logger::CLogConsole::Write(const Int8* _msg)
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

void Logger::CLogConsole::Notify(SDL_Event* _e)
{
	if (m_active)
	{
		if (_e->type == SDL_KEYDOWN)
		{
			switch (_e->key.keysym.sym)
			{
			case SDLK_0:
			case SDLK_KP_0:
				m_input.push('0');
				break;
			case SDLK_1:
			case SDLK_KP_1:
				m_input.push('1');
				break;
			case SDLK_2:
			case SDLK_KP_2:
				m_input.push('2');
				break;
			case SDLK_3:
			case SDLK_KP_3:
				m_input.push('3');
				break;
			case SDLK_4:
			case SDLK_KP_4:
				m_input.push('4');
				break;
			case SDLK_5:
			case SDLK_KP_5:
				m_input.push('5');
				break;
			case SDLK_6:
			case SDLK_KP_6:
				m_input.push('6');
				break;
			case SDLK_7:
			case SDLK_KP_7:
				m_input.push('7');
				break;
			case SDLK_8:
			case SDLK_KP_8:
				m_input.push('8');
				break;
			case SDLK_9:
			case SDLK_KP_9:
				m_input.push('9');
				break;
			case SDLK_a:
				m_input.push('a');
				break;
			case SDLK_b:
				m_input.push('b');
				break;
			case SDLK_c:
				m_input.push('c');
				break;
			case SDLK_d:
				m_input.push('d');
				break;
			case SDLK_e:
				m_input.push('e');
				break;
			case SDLK_f:
				m_input.push('f');
				break;
			case SDLK_g:
				m_input.push('g');
				break;
			case SDLK_h:
				m_input.push('h');
				break;
			case SDLK_i:
				m_input.push('i');
				break;
			case SDLK_j:
				m_input.push('j');
				break;
			case SDLK_k:
				m_input.push('k');
				break;
			case SDLK_l:
				m_input.push('l');
				break;
			case SDLK_m:
				m_input.push('m');
				break;
			case SDLK_n:
				m_input.push('n');
				break;
			case SDLK_o:
				m_input.push('o');
				break;
			case SDLK_p:
				m_input.push('p');
				break;
			case SDLK_q:
				m_input.push('q');
				break;
			case SDLK_r:
				m_input.push('r');
				break;
			case SDLK_s:
				m_input.push('s');
				break;
			case SDLK_t:
				m_input.push('t');
				break;
			case SDLK_u:
				m_input.push('u');
				break;
			case SDLK_v:
				m_input.push('v');
				break;
			case SDLK_w:
				m_input.push('w');
				break;
			case SDLK_x:
				m_input.push('x');
				break;
			case SDLK_y:
				m_input.push('y');
				break;
			case SDLK_z:
				m_input.push('z');
				break;
			case SDLK_COMMA:
				m_input.push(',');
				break;
			case SDLK_SPACE:
				m_input.push(' ');
				break;
			case SDLK_PERIOD:
			case SDLK_KP_PERIOD:
				m_input.push('.');
				break;
			case SDLK_SLASH:
				m_input.push('/');
				break;
			case SDLK_MINUS:
			case SDLK_KP_MINUS:
				m_input.push('-');
				break;
			case SDLK_BACKSPACE:
				m_input.pop();
				break;
			case SDLK_RETURN:
			case SDLK_RETURN2:
			case SDLK_KP_ENTER:
				if (!m_input.isEmpty())
				{
					InterpretInput();
				}
				break;
			default:
				break;
			}
		}
	}
}

void Logger::CLogConsole::InterpretInput()
{
	Write(m_input.buffer);

	// sample command: debug.toggle,i-p1,s-p2,b-p3
	// parameters might need a type definition in front of them? ie: i-p1,b-p2,s-p3 for int,bool,string

	// break input buffer up by looking for first period
	Int8* chr = SDL_strchr(m_input.buffer, '.');

	if (nullptr == chr)
	{
		Write("Unknown console input");
		m_input.clear();
		return;
	}

	Int16 peroid = chr - m_input.buffer + 1; 
	Int8 file[MAX_BUFFER];
	SDL_strlcpy(file, m_input.buffer, peroid);

	// buffer up to first period is name of lua file, load it
	Int8 path[MAX_BUFFER];
	SDL_snprintf(path, MAX_BUFFER, "data/lua/%s.lua", file);
	luaL_dofile(Logger::luaState, path);

	// buffer after first peroid is name of function in lua file, call this function
	// any parameters are comma separated after name of function
	++chr;

	Int8* comma = SDL_strchr(chr, ',');
	Int8 function[MAX_BUFFER];
	Int16 numParams = 0;
	Int16 numResults = 1;
	
	if (nullptr == comma)
	{
		SDL_strlcpy(function, chr, SDL_strlen(chr));
		lua_getglobal(Logger::luaState, function);
	}
	else
	{
		Int16 length = comma - chr + 1;
		SDL_strlcpy(function, chr, length);
		chr += length;
		lua_getglobal(Logger::luaState, function);

		while (nullptr != comma)
		{
			++numParams;

			Int8 type[2];
			SDL_strlcpy(type, chr, 2);

			chr = SDL_strchr(chr, '-');
			// hyphen needs to exist for command syntax to be correct
			if (nullptr == chr)
			{
				m_input.clear();
				Write("Invalid command - Parameters need a hyphen separating type and value");
			}
			++chr;

			// check for next parameter
			comma = SDL_strchr(chr, ',');
			Int8 value[MAX_BUFFER];
			if (nullptr == comma)
			{
				length = SDL_strlen(chr); // no +1 accounts for the _ at the end of the input
			}
			else 
			{
				length = (comma - chr) + 1;
			}
			SDL_strlcpy(value, chr, length);
			chr += length;

			if (!SDL_strcmp(type, "i")) // integer value
			{
				lua_pushinteger(Logger::luaState, SDL_atoi(value));
			}
			else if (!SDL_strcmp(type, "s")) // string value
			{
				lua_pushstring(Logger::luaState, value);
			}
			else if (!SDL_strcmp(type, "b")) // boolean value
			{
				lua_pushboolean(Logger::luaState, !SDL_strcmp(value, "true"));
			}
			else // unknown type
			{
				m_input.clear();
				Write("Invalid command - unrecognised parameter type. Acceptable types include: i, s, b, for integer, string, bool");
				return;
			}
		}
	}
	
	lua_call(Logger::luaState, numParams, numResults);

	m_input.clear();
	const Int8* text = lua_tolstring(Logger::luaState, -1, NULL);
	lua_pop(Logger::luaState, 1);
	Write(text);
}