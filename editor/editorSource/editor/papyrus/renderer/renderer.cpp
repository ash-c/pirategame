
// Library Includes

// Local Includes
#include "renderer.h"
#include "irenderer.h"
#include "sdlrenderer.h"

using namespace Papyrus;

Renderer::IRenderer* Renderer::activeRenderer = 0;

Bool Renderer::Initialise(Int32 _width, Int32 _height, Int8* _title, Bool _fullScreen)
{
	assert(activeRenderer == 0);
	CREATEPOINTER(activeRenderer, CSDLRenderer);

	VALIDATE(activeRenderer->Initialise(_width, _height, _title, _fullScreen));
	activeRenderer->AddRef();
	return true;
}

Bool Renderer::ShutDown()
{
	PY_DELETE_RELEASE(activeRenderer);
	return true;
}

void Renderer::Clear()
{
	assert(activeRenderer != 0);
	activeRenderer->Clear();
}

void Renderer::Present()
{
	assert(activeRenderer != 0);
	activeRenderer->Present();
}