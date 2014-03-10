#pragma once

#ifndef __PAPYRUS_UI_BUTTON_H__
#define __PAPYRUS_UI_BUTTON_H__

// Library Includes

// Local Includes
#include "../uiobject.h"
#include "../../input/observer.h"

namespace Papyrus
{
	namespace UI
	{
		typedef enum _EButtonState
		{
			BUTTON_STATE_INVALID = INVALID_ID,
			BUTTON_STATE_NORMAL,
			BUTTON_STATE_HOVER,
			BUTTON_STATE_CLICK,
			BUTTON_STATE_MAX
		} EButtonState;

		class CUIButton : public CUIObject, Input::IInputObserver
		{
			// Member Functions
		public:
			// Default Constructor
			CUIButton();

			// Default Destructor
			virtual ~CUIButton();

			virtual Bool	Initialise(Int8* _luaFile, Int8* _luaFunc, Int8* _sprite, VECTOR2 _pos, UInt16 _w, UInt16 _h);
			virtual Bool	ShutDown();
			virtual void	Process(Float32 _delta);
			virtual void	Render();

			virtual void	Notify(SDL_Event* _e);

			virtual void	SetActive(Bool _b);
			virtual void	SetPosition(VECTOR2 _pos);

			virtual Int32	GetHeight();
			virtual Int32	GetWidth();

			void			SetButtonState(EButtonState _state);
			void			SetNext(CUIButton* _next);
			void			SetPrev(CUIButton* _prev);

		private:
			Bool			CheckForHover(VECTOR2 _mouse);
			void			ButtonClicked();

			// Member Variables
		protected:
			static Float32	m_timer;

			CUIButton*		m_next;
			CUIButton*		m_prev;
			SDL_Rect		m_clips[BUTTON_STATE_MAX];
			SDL_Rect		m_rect;
			Int8			m_luaFile[MAX_BUFFER];
			Int8			m_luaFunc[MAX_BUFFER];
			EButtonState	m_currState;
			Bool			m_buttonDown;
			Bool			m_stateChanged;
		};
	}
}

#endif // __PAPYRUS_UI_BUTTON_H__