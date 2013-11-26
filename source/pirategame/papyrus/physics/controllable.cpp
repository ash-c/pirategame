
// Library Includes

// Local Includes
#include "controllable.h"
#include "../logging/logger.h"

using namespace Papyrus::Physics;

CControllable::CControllable()
{
}

CControllable::~CControllable()
{
}

Bool CControllable::Initialise()
{
	Logger::TrackValue(&m_currState.pos, "position");
	Logger::TrackValue(&m_currState.vel, "velocity");

	return true;
}

Bool CControllable::ShutDown()
{
	return true;
}