#pragma once

#ifndef __PIRATEGAME_CHARACTER_H__
#define __PIRATEGAME_CHARACTER_H__

// Library Includes

// Local Includes
#include "../core/core.h"

using namespace Papyrus;

class ICharacter
{
	// Member Functions
public:
	ICharacter() {}
	virtual ~ICharacter() {}

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings) = 0;
	virtual Bool		ShutDown() = 0;

	virtual void		Process(Float32 _delta) = 0;
	virtual void		Render(VECTOR2 _camPos) = 0;

	virtual void		SetPosition(VECTOR2 _v) = 0;
	virtual VECTOR2		GetPosition() { return m_pos; }

	// Member Variables
protected:
	static const Int32	SPEED = 400;

	Sprite::ISprite*	m_sprite;
	VECTOR2				m_pos;
};

#endif // __PIRATEGAME_CHARACTER_H__