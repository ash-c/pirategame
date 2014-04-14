#pragma once

#ifndef __PAPYRUS_SOUND_H__
#define __PAPYRUS_SOUND_H__

// Library Includes
//#include <fmod.h>
#include <fmod.hpp>

// Local Includes
#include "../defines.h"

namespace Papyrus
{
	namespace Sound
	{
		/*
		* Initialises the sound system.
		*
		* @return	Returns true when initialised successfully, false otherwise.
		*/
		Bool		Initialise();

		/*
		* Shuts the sound system down and cleans memory.
		*
		* @return	Returns true on success, false otherwise.
		*/
		Bool		ShutDown();

		Bool		PlayBkgMusic(Int8* _path);

		extern FMOD::System*		system;

		extern FMOD::Sound*			bkgMusic;
		extern FMOD::Channel*		bkgChannel;
		extern Int8					bkgMusicPath[MAX_BUFFER];
	}
}

#endif // __PAPYRUS_SOUND_H__