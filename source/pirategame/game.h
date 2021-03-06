
#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// Library Includes

// Local Includes
#include "../papyrus\core\utility\singleton.h"
#include "../papyrus\core\core.h"

#include "level\levelmanager.h"

using namespace Papyrus;

// Prototypes
class ICharacter;

class CGame : public TSingleton<CGame>, Input::IInputObserver
{
	// Friending is required for the singleton template to work.
	friend class TSingleton<CGame>;

	// Member Functions
public:
	// Default Destructor
	virtual		~CGame();

	/*
	* Executes one frame of the game.
	*
	* @return	Returns void.
	*/
	void		ExecuteOneFrame();

	/*
	* Initialise the game.
	*
	* @return	Returns true if successful, false otherwise.
	*/
	Bool		Initialise();

	/*
	* Processes the game.
	*
	* @return	Returns void.
	*/
	void		Process(Float32 _delta);

	/*
	* Renders the game to screen.
	*
	* @return	Returns void.
	*/
	void		Render();

	/*
	* Checks if the game is still active.
	*
	* @return	Returns true if the game is active, false otherwise.
	*/
	Bool		IsActive();

	void		Pause();
	Bool		IsPaused() { return m_paused; } 

	virtual void		Notify(SDL_Event* _e);

	/*
	* Quits the game, used via a command in the debug console.
	*
	* @return	Returns the number of variables on the lua stack.
	*/
	static Int32		QuitGame(lua_State* L);
	static Int32		TitleMenu(lua_State* L);
	static Int32		StartGame(lua_State* L);
	static Int32		PauseGame(lua_State* L);
	static Int32		LoadLevel(lua_State* L);
	static Int32		ToggleCursor(lua_State* L);

protected:
	// Default Constructor
	CGame();

	// Member Variables
protected:
	CLevelManager*				m_levelMan;
	UI::IUIInterface*			m_interface;

	Int8*						m_currLevel;
	Int8*						m_startLevel;

	Bool						m_active;
	Bool						m_paused;
	Bool						m_gameRunning;
};
#define GAME CGame::GetInstance();

#endif // __GAME_H__