
// Library Includes
#include <SDL.h>

// Local Includes
#include "game.h"
#include "editor\editor.h"
#include "editor\leveledit.h"
#include "editor\interfaceedit.h"

CGame::CGame()
	: m_interface(0)
	, m_editor(0)
	, m_active(true)
	, m_paused(false)
{

}

CGame::~CGame()
{
	m_interface->Release();
	PY_DELETE_RELEASE(m_editor);
	Core::ShutDown();
}

void CGame::ExecuteOneFrame()
{
	Process(Core::Process());
	Render();
}

Bool CGame::Initialise()
{
	VALIDATE(Core::Initialise());

	// Register the quit function called via the debug console
	lua_register(Logger::luaState, "Quit", Quit);
	lua_register(Logger::luaState, "StartLevelEditor", StartLevelEditor);
	lua_register(Logger::luaState, "StartInterfaceEditor", StartInterfaceEditor);

	VALIDATE(Input::inputManager->Register(this));

	m_interface = UI::LoadInterface("data/interfaces/editorMain.ini");
	m_interface->AddRef();

	// Finish setup logging, open error logging file.
	PY_WRITETOFILE("Initialistion complete");
	Logger::InitFile("data/papyrus/editorErrors.log");

	return true;
}

void CGame::Process(Float32 _delta)
{
	if (0 != m_editor) m_editor->Process(_delta);
}

void CGame::Render()
{
	Renderer::Clear();

	if (0 != m_editor) m_editor->Render();
		
	if (0 != m_interface) m_interface->Render();
	
	// Render core last, as it renders logging and physics debug, in that order.
	Core::Render();

	Renderer::Present();
}

Bool CGame::IsActive()
{
	return m_active;
}

void CGame::Notify(SDL_Event* _e)
{
	if (_e->type == SDL_QUIT)
	{
		m_active = false;			
	}
	else if (_e->type == SDL_KEYDOWN)
	{
		switch (_e->key.keysym.sym)
		{
		case SDLK_ESCAPE:
			m_active = false;
			break;
		case SDLK_F1:
			m_editor->Save();
			break;
		case SDLK_BACKQUOTE: // `
			Logger::ToggleConsole(nullptr);
			break;
		default:
			break;
		}
	}
}

Int32 CGame::Quit(lua_State* L)
{
	sm_pTheInstance->m_active = false;
	return 0;
}

Int32 CGame::StartLevelEditor(lua_State* L)
{
	Logger::Write("Level Editor Started");
	if (0 != sm_pTheInstance->m_editor) 
	{
		PY_DELETE_RELEASE(sm_pTheInstance->m_editor);
	}

	CREATEPOINTER(sm_pTheInstance->m_editor, CLevelEdit);
	assert(sm_pTheInstance->m_editor);

	if (sm_pTheInstance->m_editor->Initialise()) 
	{
		sm_pTheInstance->m_interface->Toggle();
		sm_pTheInstance->m_interface->Release();
		sm_pTheInstance->m_editor->AddRef();
	}

	return 0;
}

Int32 CGame::StartInterfaceEditor(lua_State* L)
{
	Logger::Write("Interface Editor Started");
	if (0 != sm_pTheInstance->m_editor) 
	{
		PY_DELETE_RELEASE(sm_pTheInstance->m_editor);
	}

	CREATEPOINTER(sm_pTheInstance->m_editor, CIntEdit);
	assert(sm_pTheInstance->m_editor);

	if (sm_pTheInstance->m_editor->Initialise()) 
	{
		sm_pTheInstance->m_interface->Toggle();
		sm_pTheInstance->m_interface->Release();
		sm_pTheInstance->m_editor->AddRef();
	}

	return 0;
}