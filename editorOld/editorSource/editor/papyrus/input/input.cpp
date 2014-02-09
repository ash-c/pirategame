
// Library Includes

// Local Includes
#include "input.h"

using namespace Papyrus;

Input::CInputManager*	Input::inputManager = 0;

Bool Input::Initialise()
{
	CREATEPOINTER(inputManager, CInputManager);
	assert(inputManager);
	inputManager->Initialise();
	return true;
}

Bool Input::ShutDown()
{
	inputManager->ShutDown();
	CLEANDELETE(inputManager);

	return true;
}