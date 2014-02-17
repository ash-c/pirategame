#pragma once

#ifndef __EDITOR_TOOL_TILETOOL_H__
#define __EDITOR_TOOL_TILETOOL_H__

// Library Includes

// Local Includes
#include "../tool.h"

class CToolTile : public ITool
{
	// Member Functions
public:
	CToolTile();
	virtual ~CToolTile();

	virtual Bool		Initialise();
	virtual Bool		ShutDown();

	virtual Bool		AddToLevel(CLevel* _level);

	// Member Variables
protected:
};

#endif // __EDITOR_TOOL_TILETOOL_H__