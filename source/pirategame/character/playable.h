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

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo);
	virtual Bool		ShutDown();

	virtual void		Process(Float32 _delta);
	virtual void		Render();

	virtual void		Notify(SDL_Event* _e);

	// Member Variables
protected:
	typedef enum _EAnims
	{
		INVALID_ANIM = INVALID_ID,
		ANIM_IDLE_LEFT,
		ANIM_IDLE_RIGHT,
		ANIM_RUN_LEFT,
		ANIM_RUN_RIGHT,
		ANIM_DEATH_LEFT,
		ANIM_DEATH_RIGHT,
		ANIM_ATTACK_LEFT,
		ANIM_ATTACK_RIGHT,
		ANIM_JUMP_LEFT,
		ANIM_JUMP_RIGHT,
		ANIM_LADDER_CLIMB,
		ANIM_VINE_LEFT,
		ANIM_VINE_RIGHT,
		MAX_ANIMS
	} EAnims;

	EAnims			m_currAnim;
};

#endif // __PIRATEGAME_CHARACTER_PLAYABLE_H__