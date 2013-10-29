
#pragma once

#ifndef __PAPYRUS_CORE_H__
#define __PAPYRUS_CORE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "../parser/parser.h"
#include "../renderer/renderer.h"
#include "../sprite/sprite.h"

namespace Papyrus
{
	namespace Core
	{
		Bool Initialise(Int32 _numParsers)
		{
			assert(_numParsers > 0 && "Need more than 0 parsers");
			FileParser::maxNumParsers = _numParsers;
			FileParser::Initialise();
			FileParser::IParser* setup = FileParser::LoadFile("data/setup.ini");
			assert(setup);
			setup->AddRef();
			Int32 width, height;
			Int8* title = 0;
			Bool fullscreen;
			VALIDATE(setup->GetValue("width", width, "screen"));
			VALIDATE(setup->GetValue("height", height, "screen"));
			VALIDATE(setup->GetValue("fullscreen", fullscreen, "screen"));
			VALIDATE(setup->GetValue("title", &title, "screen"));

			VALIDATE(!FileParser::FlushFile(setup));

			VALIDATE(Renderer::Initialise(width, height, title, fullscreen));
			CLEANDELETE(title);

			VALIDATE(Sprite::Initialise());

			return true;
		}

		Float32 Process()
		{
			return 0.0f;
		}

		Bool ShutDown()
		{
			VALIDATE(Sprite::ShutDown());
			VALIDATE(FileParser::ShutDown());
			VALIDATE(Renderer::ShutDown());
			SDL_Quit();
			return true;
		}
	}
}

#endif // __PAPYRUS_CORE_H__