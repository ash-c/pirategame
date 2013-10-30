//
// refcount.h
//
// This file defines an interface for giving objects ref counts.
//
#pragma once

#ifndef __PAPYRUS_REFCOUNT_H__
#define __PAPYRUS_REFCOUNT_H__

// Library Includes

// Local Includes
#include "../defines.h"

template <typename T>
class TRefCount
{
	// Member Functions
public:
	TRefCount()
		: m_refCount(0)
	{
	}

	virtual ~TRefCount()
	{
			assert(m_refCount == 0 && "Ref count is not 0, did you forget to call Release somewhere?");
	}

	/*
	* Increase the reference counter. Must be called when the parser is being used by a class.
	*/
	virtual void AddRef()
	{
		++m_refCount;
	}

	/*
	* Decreases the reference counter. Must be called when an object no longer requires the parser.
	*/
	virtual void Release()
	{
		assert(m_refCount > 0 && "Ref count is not greater than 0, can't release. Did you forget to call AddRef somewhere?");
		--m_refCount;
	}

	private:
		TRefCount(const TRefCount& _rhs);
		TRefCount& operator = (const TRefCount& _rhs);

	// Member Variables
private:
	UInt16					m_refCount;
};

#endif // __PAPYRUS_REFCOUNT_H__