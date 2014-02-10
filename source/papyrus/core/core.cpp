
// Library Includes

// Local Includes
#include "core.h"
#include "../parser/ini/iniparser.h"

using namespace Papyrus;

Timer::CTimer* Core::timer = 0;

Bool Core::Initialise()
{
	FileParser::CIniparser setup;
	setup.Initialise("data/papyrus/setup.ini");
	setup.Load();

	Int32 width, height, numParsers, numSprites, numActors, numInterfaces;
	Int8* title = 0;
	Bool fullscreen;

	setup.AddRef();
	VALIDATE(setup.GetValue("parsers", numParsers, "init"));
	VALIDATE(setup.GetValue("sprites", numSprites, "init"));
	VALIDATE(setup.GetValue("actors", numActors, "init"));
	VALIDATE(setup.GetValue("interfaces", numInterfaces, "init"));
	VALIDATE(setup.GetValue("width", width, "screen"));
	VALIDATE(setup.GetValue("height", height, "screen"));
	VALIDATE(setup.GetValue("fullscreen", fullscreen, "screen"));
	VALIDATE(setup.GetValue("title", &title, "screen"));
	setup.Release();

	VALIDATE(Logger::Initialise());
	VALIDATE(Logger::InitFile("data/papyrus/startup.log"));
	PY_WRITETOFILE("Initialisation started");
	PY_WRITETOFILE("Logging sub-system initialised");

	FileParser::maxNumParsers = numParsers;
	VALIDATE(FileParser::Initialise());
	PY_WRITETOFILE("File parsing sub-system initialised");

	VALIDATE(Renderer::Initialise(width, height, title, fullscreen));
	CLEANARRAY(title); // This is needed as the parser allocates memory on the heap.
	PY_WRITETOFILE("Rendering sub-system initialised");

	//SDL_ShowCursor(SDL_DISABLE);

	Sprite::maxNumSprites = numSprites;
	VALIDATE(Sprite::Initialise());
	PY_WRITETOFILE("Sprite sub-system initialised");

	VALIDATE(Input::Initialise());
	PY_WRITETOFILE("Input sub-system initialised");
	
	Physics::maxActors = numActors;
	VALIDATE(Physics::Initialise());
	PY_WRITETOFILE("Physics sub-system initialised");

	UI::numInterfaces = numInterfaces;
	VALIDATE(UI::Initialise());
	PY_WRITETOFILE("User Interface sub-system initialised");

	CREATEPOINTER(timer, Timer::CTimer);
	VALIDATE(timer->Initialise());
	PY_WRITETOFILE("Timing sub-system initialised");

	timer->Start();

	Logger::Write("Core initialisation successfull", NULL);
	return true;
}

Float32 Core::Process()
{
	Float32 delta = 0.0f;

	if (!timer->Paused())
	{
		delta = timer->Restart();
		Physics::Process(delta);
	}

	Input::inputManager->Process(delta);

	Logger::Process(delta);

	return delta;
}

void Core::Render()
{
	Logger::Render();
	Physics::RenderDebug();
}

Bool Core::ShutDown()
{
	CLEANDELETE(timer);
	VALIDATE(UI::ShutDown());
	VALIDATE(Physics::ShutDown());
	VALIDATE(Input::ShutDown());
	VALIDATE(Sprite::ShutDown());
	VALIDATE(FileParser::ShutDown());
	VALIDATE(Renderer::ShutDown());
	VALIDATE(Logger::ShutDown());
	SDL_Quit();
	return true;
}

void Core::Pause()
{
	if (timer->Paused())
	{
		timer->UnPause();
	}
	else
	{
		timer->Pause();
	}
}