//
// refcount.h
//
// This file defines an interface for giving objects ref counts.
//
#pragma once

#ifndef __PAPYRUS_REFCOUNT_H__
#define __PAPYRUS_REFCOUNT_H__

// Library Includes
#include <SDL.h>

// Local Includes
#include "../defines.h"

template <typename T>
class TRefCount
{
	// Member Functions
public:
	// Default Constructor
	TRefCount()
		: m_refCount(0)
		, m_filePath(0)
	{
	}

	// Default Destructor
	virtual ~TRefCount()
	{
			CLEANARRAY(m_filePath);
			assert(m_refCount == 0 && "Ref count is not 0, did you forget to call Release somewhere?");
	}

	/*
	* Increase the reference counter. Must be called when the parser is being used by a class.
	*
	* @return	Returns void.
	*/
	virtual void AddRef()
	{
		++m_refCount;
	}

	/*
	* Decreases the reference counter. Must be called when an object no longer requires the parser.
	*
	* @return	Returns void.
	*/
	virtual void Release()
	{
		assert(m_refCount > 0 && "Ref count is not greater than 0, can't release. Did you forget to call AddRef somewhere?");
		--m_refCount;
	}

	/*
	* Compares the given file path against the one loaded into this parser.
	*
	* @param	_path		Path to compare.
	* @return	Returns true if _path matches this parser ie:that file has been loaded, false otherwise.
	*/
	virtual Bool CompareFilePath(const Int8* _path)
	{
		return(!SDL_strcmp(m_filePath, _path));
	}

	private:
		TRefCount(const TRefCount& _rhs);
		TRefCount& operator = (const TRefCount& _rhs);

	// Member Variables
protected:
	Int8*				m_filePath;

private:
	UInt16				m_refCount;
};

#endif // __PAPYRUS_REFCOUNT_H__