
// Library Includes

// Local Includes
#include "interactive.h"
#include "../parser/parser.h"
#include "objects/uibutton.h"

using namespace Papyrus;

UI::CInteractiveUI::CInteractiveUI()
{
}

UI::CInteractiveUI::~CInteractiveUI()
{
}

Bool UI::CInteractiveUI::Initialise(Int8* _path)
{
	assert(0 != _path);
	Int16 len = SDL_strlen(_path) + 1;
	m_filePath = new Int8[len];
	SDL_strlcpy(m_filePath, _path, len);

	FileParser::IParser* setup = FileParser::LoadFile(_path);
	assert(setup);
	setup->AddRef();

	UInt32 numButtons = 0, numStatic = 0;
	Int8* luaFile = 0;
	VALIDATE(setup->GetValue("buttons", numButtons, "info"));
	VALIDATE(setup->GetValue("static", numStatic, "info"));
	VALIDATE(setup->GetValue("lua", &luaFile, "info"));

	// Will be other types of objects.
	m_numObjects = numButtons + numStatic;

	m_objects = new CUIObject*[m_numObjects];
	assert(m_objects);
	SDL_memset(m_objects, 0, sizeof(CUIObject*) * m_numObjects);
	
	UInt32 w = 0, h = 0;
	VECTOR2 pos;
	Int8* spritePath = 0;
	Int8* luaFunc = 0;
	Int8 buffer[MAX_BUFFER];
	UInt16 count = 0;
	
	// Make static images.
	UInt16 k;
	for (k = 0; k < numStatic; ++k)
	{
		++count;
	}

	// Make buttons.
	for (k = 0; k < numButtons; ++k)
	{
		CREATEPOINTER(m_objects[count], CUIButton);
		assert(m_objects[count]);

		SDL_snprintf(buffer, MAX_BUFFER, "button%i", k + 1);
		setup->GetValue("x", pos.x, buffer);
		setup->GetValue("y", pos.y, buffer);
		setup->GetValue("w", w, buffer);
		setup->GetValue("h", h, buffer);
		setup->GetValue("sprite", &spritePath, buffer);
		setup->GetValue("lua", &luaFunc, buffer);

		m_objects[count]->Initialise(luaFile, luaFunc, spritePath, pos, w, h);
		m_objects[count]->AddRef();
		CLEANARRAY(spritePath);
		CLEANARRAY(luaFunc);
		++count;
	}
	CLEANARRAY(luaFile);

	setup->Release();

	return true;
}

Bool UI::CInteractiveUI::ShutDown()
{
	VALIDATE(IUIInterface::ShutDown());
	return true;
}

void UI::CInteractiveUI::Render()
{
	for (UInt16 i = 0; i < m_numObjects; ++i)
	{
		if (m_objects[i]->IsActive())
		{
			m_objects[i]->Render();
		}
	}
}