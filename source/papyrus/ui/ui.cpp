
// Library Includes
#include <SDL.h>

// Local Includes
#include "ui.h"
#include "staticui.h"
#include "interactive.h"
#include "context.h"

using namespace Papyrus;

UI::IUIInterface**		UI::interfaces = 0;
extern Sprite::ISprite* UI::loadingScreen = 0;
Float32					UI::hScale = 0.0f;
Float32					UI::wScale = 0.0f;
UInt16					UI::numInterfaces = 10;		

Bool UI::Initialise() 
{
	assert(numInterfaces > 0);

	interfaces = new IUIInterface*[numInterfaces];
	assert(interfaces);
	SDL_memset(interfaces, 0, sizeof(IUIInterface*) * numInterfaces);

	hScale = Renderer::activeRenderer->GetHeight() / 1080.0f;
	wScale = Renderer::activeRenderer->GetWidth() / 1920.0f;

	return true;
}

Bool UI::ShutDown()
{
	PY_SAFE_RELEASE(loadingScreen);	
	PY_CLEANARRAY(interfaces, numInterfaces);
	CLEANARRAY(interfaces);
	return true;
}

UI::IUIInterface* UI::CreateInterface(Int8* _path, Bool _interactive, Bool _context)
{
	assert(interfaces && "Can't create interface, array missing");
	assert(0 != _path && "Path missing for interface creation");

	// First check that interface has not already been created.
	for (Int32 i = 0; i < numInterfaces; ++i)
	{
		if (0 != interfaces[i])
		{
			if (interfaces[i]->CompareFilePath(_path))
			{
				return interfaces[i]; // Return the interface that already exists for that file.
			}
		}
	}

	IUIInterface* ui = 0;
	if (_context)
	{
		CREATEPOINTER(ui, CContextMenu);
	}
	if (_interactive)
	{
		CREATEPOINTER(ui, CInteractiveUI);
	}
	else
	{
		CREATEPOINTER(ui, CStaticUI);
	}
	assert(ui);

	for (Int16 i = 0; i < numInterfaces; ++i)
	{
		if (0 == interfaces[i])
		{
			interfaces[i] = ui;
			VALIDATE(ui->Initialise(_path));
			return ui;
		}
	}
	// No free space!
	PY_DELETE_RELEASE(ui);
	assert(0 && "Set numInterfaces to be a larger value - no room with current value");
	return 0;
}

UI::IUIInterface* UI::LoadInterface(Int8* _filePath, Bool _interactive, Bool _context)
{
	assert(interfaces && "Can't create interface, array missing");
	assert(0 != _filePath && "Path missing for interface creation");

	// First check that interface has not already been created.
	for (Int32 i = 0; i < numInterfaces; ++i)
	{
		if (0 != interfaces[i])
		{
			if (interfaces[i]->CompareFilePath(_filePath))
			{
				if (!interfaces[i]->IsActive())
				{
					interfaces[i]->Toggle();
				}
				return interfaces[i]; // Return the interface that already exists for that file.
			}
		}
	}

	IUIInterface* ui = CreateInterface(_filePath, _interactive, _context);
	assert(ui && "Inteface creation failed");

	if (ui)
	{
		return ui;
	}
	return 0;
}

UI::IUIInterface* UI::FlushInterface(IUIInterface* _interface)
{	
	if (0 != interfaces)
	{
		for (Int16 i = 0; i < numInterfaces; ++i)
		{
			if (_interface == interfaces[i])
			{
				PY_DELETE_RELEASE(interfaces[i]);
				return 0;
			}
		}
	}

	return _interface;
}

void UI::LoadingScreen()
{
	if (0 == loadingScreen)
	{
		loadingScreen = Sprite::CreateSprite("data/art/ui/loadingScreen.png", 0, false);
		assert(loadingScreen);
		loadingScreen->AddRef();
		loadingScreen->SetPosition(static_cast<Int32>(Renderer::activeRenderer->GetWidth() * 0.5f),
			static_cast<Int32>(Renderer::activeRenderer->GetHeight() * 0.5f));
	}

	Renderer::Clear();
	loadingScreen->Render();
	Renderer::Present();
}