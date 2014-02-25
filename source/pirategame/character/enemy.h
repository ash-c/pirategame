#pragma once

#ifndef __PAPYRUS_CHARACTER_ENEMY_H__
#define __PAPYRUS_CHARACTER_ENEMY_H__

// Library Includes

// Local Includes
#include "character.h"

class CEnemy : public ICharacter
{
	// Member Functions
public:
	CEnemy();
	virtual ~CEnemy();

	virtual Bool		Initialise(Int8* _spriteSheet, Int8* _spriteInfo, Int8* _settings);
	virtual Bool		ShutDown();

	virtual void		Process(Float32 _delta);
	virtual void		Render(VECTOR2 _camPos);

	virtual void		SetPosition(VECTOR2 _v);

	Bool				IsActive() { return m_actor->IsActive(); }

	// Member Variables
protected:
	Float32				m_attackDelay;
	Int32				m_screenW;
	Int32				m_screenH;

	Bool				m_left;
};

#endif // __PAPYRUS_CHARACTER_ENEMY_H__