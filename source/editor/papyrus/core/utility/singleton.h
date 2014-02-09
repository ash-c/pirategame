
#pragma once

#ifndef __PAPYRUS_SINGLETON_H__
#define __PAPYRUS_SINGLETON_H__

// Library Includes

// Local Includes
#include "../defines.h"

// Types

// Constants

// Prototypes

template <typename T>
class TSingleton
{
	// Member Functions
public:
	static T& GetInstance()
	{
		CREATEPOINTER(sm_pTheInstance, T)
		return(*sm_pTheInstance);
	}

	static void DestroyInstance()
	{
		CLEANDELETE(sm_pTheInstance)
	}

protected:
	TSingleton()
	{
	}

	virtual ~TSingleton()
	{
	}

private:
	TSingleton(const TSingleton& _rhs)
	{
	}

	TSingleton& operator = (const TSingleton& _rhs)
	{
	}

	// Member Variables
protected:
	static T* sm_pTheInstance;
};

template <typename T> T* TSingleton<T>::sm_pTheInstance = 0;

#endif // __PAPYRUS_SINGLETON_H__