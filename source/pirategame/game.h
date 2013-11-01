
#pragma once

#ifndef __GAME_H__
#define __GAME_H__

// Library Includes

// Local Includes
#include "papyrus\core\utility\singleton.h"
#include "papyrus\core\core.h"

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
	void		Process();

	/*
	* Renders the game to screen.
	*
	* @return	Returns void.
	*/
	void		Render();

	Bool		IsActive();

protected:
	// Default Constructor
	CGame();

private:
	CGame(const CGame& _rhs);
	CGame& operator = (const CGame& _rhs);

	// Member Variables
private:
	Bool						active;
};
#define GAME CGame::GetInstance();

#endif // __GAME_H__