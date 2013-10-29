
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
	assert(activeRenderer && "Failed to create the SDL renderer");

	VALIDATE(activeRenderer->Initialise(_width, _height, _title, _fullScreen));
	activeRenderer->AddRef();
	return true;
}

Bool Renderer::ShutDown()
{
	assert(activeRenderer != 0);
	
	activeRenderer->Release();
	return activeRenderer->ShutDown();
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