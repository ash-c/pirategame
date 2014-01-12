#pragma once

#ifndef __PAPYRUS_UI_INTERFACE_H__
#define __PAPYRUS_UI_INTERFACE_H__

// Library Includes

// Local Includes
#include "../defines.h"
#include "../core/utility/refcount.h"
#include "uiobject.h"

namespace Papyrus
{
	namespace UI
	{
		class IUIInterface : public TRefCount<IUIInterface>
		{
			// Member Functions
		public:
			// Default constructor
			IUIInterface()
				: m_objects(0)
				, m_numObjects(0)
				, m_active(true)
			{
			}

			// Default Destructor
			virtual ~IUIInterface()
			{
			}

			/*
			* Initialise the interface.
			*
			* @return		Returns true on successfull intialisation, false otherwise.
			*/
			virtual	Bool	Initialise(Int8* _path) = 0;

			/*
			* Cleans memory and shuts the interface down.
			*
			* @return		Returns true on success, false otherwise.
			*/
			virtual Bool	ShutDown()
			{
				for (UInt16 i = 0; i < m_numObjects; ++i)
				{
					PY_DELETE_RELEASE(m_objects[i]);
				}
				CLEANARRAY(m_objects);

				return true;
			}

			virtual Bool	Toggle()
			{
				m_active = !m_active;

				for (UInt16 i = 0; i < m_numObjects; ++i)
				{
					m_objects[i]->SetActive(m_active);
				}

				return m_active;
			}

			virtual Bool	IsActive()
			{
				return m_active;
			}

			/*
			* Renders this interface to screen.
			*
			* @return		Returns void.
			*/
			virtual void	Render() = 0;

			// Member Variables
		protected:
			CUIObject**		m_objects;
			UInt32			m_numObjects;
			Bool			m_active;
		};
	}
}

#endif // __PAPYRUS_UI_INTERFACE_H__