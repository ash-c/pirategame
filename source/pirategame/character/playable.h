#pragma once

#ifndef __PIRATEGAME_CHARACTER_PLAYABLE_H__
#define __PIRATEGAME_CHARACTER_PLAYABLE_H__

// Library Includes

// Local Includes
#include "character.h"

class CPlayable : public ICharacter, public Papyrus::Input::IInputObserver
{
	// Member Functions
public:
	CPlayable();
	virtual ~CPlayable();

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings);
	virtual Bool		ShutDown();

	virtual void		Process(Float32 _delta);
	virtual void		Render(VECTOR2 _camPos);

	virtual void		SetPosition(VECTOR2 _v);

	virtual void		Notify(SDL_Event* _e);

protected:
	virtual void		Move(VECTOR2* _vel, Bool _left);
	virtual void		StopMove(Bool _left);
	virtual void		Jump(VECTOR2* _vel);
	virtual void		Attack();

	// Member Variables
protected:
	typedef enum _EMovement
	{
		INVALID_MOVE = INVALID_ID,
		MOVE_IDLE,
		MOVE_RIGHT,
		MOVE_LEFT,
		MAX_MOVE
	} EMovement;

	Float32					m_startDelay;
	EMovement				m_moveDir;
};

#endif // __PIRATEGAME_CHARACTER_PLAYABLE_H__