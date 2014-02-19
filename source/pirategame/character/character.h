#pragma once

#ifndef __PIRATEGAME_CHARACTER_H__
#define __PIRATEGAME_CHARACTER_H__

// Library Includes

// Local Includes
#include "../../core/core.h"

using namespace Papyrus;

class ICharacter
{
	// Member Functions
public:
	typedef enum _EAnims
	{
		INVALID_ANIM = INVALID_ID,
		ANIM_IDLE_LEFT,
		ANIM_IDLE_RIGHT,
		ANIM_RUN_LEFT,
		ANIM_RUN_RIGHT,
		ANIM_ATTACK_LEFT,
		ANIM_ATTACK_RIGHT,
		ANIM_DEATH_LEFT,
		ANIM_DEATH_RIGHT,
		ANIM_JUMP_LEFT,
		ANIM_JUMP_RIGHT,
		ANIM_FALL_LEFT,
		ANIM_FALL_RIGHT,
		ANIM_SLIDE_LEFT,
		ANIM_SLIDE_RIGHT,
		MAX_ANIMS
	} EAnims;

	ICharacter() 
		: m_sprite(0)
		, m_actor(0)
		, m_currAnim(ANIM_IDLE_RIGHT)
	{}
	virtual ~ICharacter() {}

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings) = 0;
	virtual Bool		ShutDown() = 0;

	virtual void		Process(Float32 _delta) = 0;
	virtual void		Render(VECTOR2 _camPos) = 0;

	virtual void		SetPosition(VECTOR2 _v) = 0;
	virtual VECTOR2		GetPosition() { return m_pos; }

	virtual Float32		GetWidth() { return m_sprite->GetScale().x; }
	virtual Float32		GetHeight() { return m_sprite->GetScale().y; }

	virtual Papyrus::Physics::EType GetType() { return m_actor->GetType(); }

	// Member Variables
protected:
	Sprite::ISprite*		m_sprite;
	Physics::IDynamicActor*	m_actor;

	VECTOR2					m_pos;

	VECTOR2					m_moveForce;
	VECTOR2					m_slowDownForce;

	EAnims					m_currAnim;

#ifdef PAPYRUS_EDITOR
public:
	Int8				m_spriteSheet[MAX_BUFFER];
	Int8				m_spriteSettings[MAX_BUFFER];
	Int8				m_settings[MAX_BUFFER];
#endif // PAPYRUS_EDITOR
};

#endif // __PIRATEGAME_CHARACTER_H__