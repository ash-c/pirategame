
// Library Includes

// Local Includes
#include "core.h"
#include "../parser/ini/iniparser.h"

using namespace Papyrus;

Timer::CTimer* Core::timer = 0;

Bool Core::Initialise()
{
	FileParser::CIniparser setup;
	setup.Initialise("data/setup.ini");
	setup.Load();

	Int32 width, height, numParsers;
	Int8* title = 0;
	Bool fullscreen;

	setup.AddRef();
	VALIDATE(setup.GetValue("parsers", numParsers, "init"));
	VALIDATE(setup.GetValue("width", width, "screen"));
	VALIDATE(setup.GetValue("height", height, "screen"));
	VALIDATE(setup.GetValue("fullscreen", fullscreen, "screen"));
	VALIDATE(setup.GetValue("title", &title, "screen"));
	setup.Release();

	VALIDATE(Logger::Initialise());
	VALIDATE(Logger::InitFile("data/startup.log"));
	PY_WRITETOFILE("Logging initialised\n");

	FileParser::maxNumParsers = numParsers;
	VALIDATE(FileParser::Initialise());
	PY_WRITETOFILE("File parsing initialised\n");

	VALIDATE(Renderer::Initialise(width, height, title, fullscreen));
	CLEANDELETE(title);
	PY_WRITETOFILE("Renderer initialised\n");

	VALIDATE(Sprite::Initialise());
	PY_WRITETOFILE("Sprites initialised\n");

	CREATEPOINTER(timer, Timer::CTimer);
	VALIDATE(timer->Initialise());
	PY_WRITETOFILE("Logger initialised\n");

	Logger::Write("Core Initialised", NULL);

	timer->Start();

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