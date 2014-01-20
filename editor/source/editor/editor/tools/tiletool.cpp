
// Library Includes

// Local Includes
#include "tiletool.h"

CToolTile::CToolTile()
{
}

CToolTile::~CToolTile()
{
}

Bool CToolTile::Initialise()
{
	return true;
}

Bool CToolTile::ShutDown()
{
	return true;
}

Bool CToolTile::AddToLevel(CLevel* _level, VECTOR2 _pos)
{
	return _level->AddTile(_pos);
}