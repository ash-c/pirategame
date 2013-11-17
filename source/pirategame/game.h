
#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// Library Includes

// Local Includes
#include "papyrus\core\utility\singleton.h"
#include "papyrus\core\core.h"

using namespace Papyrus;

// Prototypes

class CGame : public TSingleton<CGame>
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

	/*
	* Quits the game, used via a command in the debug console.
	*
	* @return	Returns the number of variables on the lua stack.
	*/
	static Int32		QuitGame(lua_State* L);

protected:
	// Default Constructor
	CGame();

private:
	CGame(const CGame& _rhs);
	CGame& operator = (const CGame& _rhs);

	// Member Variables
protected:
	Sprite::ISprite*			m_background;
	Sprite::ISprite*			m_anim;
	Bool						m_active;
};
#define GAME CGame::GetInstance();

#endif // __GAME_H__