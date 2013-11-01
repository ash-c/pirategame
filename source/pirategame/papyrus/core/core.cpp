
// Library Includes

// Local Includes
#include "core.h"

using namespace Papyrus;

Timer::CTimer* Core::timer = 0;

Bool Core::Initialise(Int32 _numParsers)
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

	VALIDATE(Logger::Initialise());

	VALIDATE(Renderer::Initialise(width, height, title, fullscreen));
	CLEANDELETE(title);

	VALIDATE(Sprite::Initialise());

	CREATEPOINTER(timer, Timer::CTimer);
	VALIDATE(timer->Initialise());
	timer->Start();

	Logger::Write("Core Initialised", NULL);

	return true;
}

Float32 Core::Process()
{
	Float32 fDelta = timer->Restart();

	Logger::Process(fDelta);

	return fDelta;
}

void Core::Render()
{
	Logger::Render();
}

Bool Core::ShutDown()
{
	CLEANDELETE(timer);
	VALIDATE(Sprite::ShutDown());
	VALIDATE(Logger::ShutDown());
	VALIDATE(FileParser::ShutDown());
	VALIDATE(Renderer::ShutDown());
	SDL_Quit();
	return true;
}