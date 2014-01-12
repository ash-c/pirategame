
#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// Library Includes

// Local Includes
#include "papyrus\core\utility\singleton.h"
#include "papyrus\core\core.h"

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

	virtual void		Notify(SDL_Event* _e);

protected:
	// Default Constructor
	CGame();

	// Member Variables
protected:
	UI::IUIInterface*			m_interface;

	Bool						m_active;
	Bool						m_paused;
};
#define GAME CGame::GetInstance();

#endif // __GAME_H__