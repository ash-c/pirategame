
// Library Includes

// Local Includes
#include "water.h"

CWater::CWater()
{
}

CWater::~CWater()
{
}

Bool CWater::Initialise(Int8* _spritesheet, VECTOR2 _pos, ETileType _type)
{
	VALIDATE(CTile::Initialise(_spritesheet, _pos, _type));

	m_actor->SetType(Papyrus::Physics::EType::TYPE_WATER);

	return true;
}