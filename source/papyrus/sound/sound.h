#pragma once

#ifndef __PAPYRUS_SOUND_H__
#define __PAPYRUS_SOUND_H__

// Library Includes
//#include <fmod.h>
#include <fmod.hpp>
#include <map>

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

		/*
		* Plays background music.
		*
		* @param	_path			The path to the music file to play.
		* @return	Returns true on success, false otherwise.
		*/
		Bool		PlayBkgMusic(Int8* _path);

		/*
		* Preloads a sound effect ready for use.
		*
		* @param	_path			The path to the sound effect to play.
		* @return Returns true on success, false otherwise.
		*/
		Bool		PreLoadSFX(Int8* _path);

		/*
		* Plays a sound effect.
		*
		* @param	_path			The path to the sound effect to play.
		* @return	Returns true on success, false otherwise.
		*/
		Bool		PlaySFX(Int8* _path);

		/*
		* Variables.
		*/

		// FMOD system
		extern FMOD::System*		system;

		// Background music
		extern FMOD::Sound*			bkgMusic;
		extern FMOD::Channel*		bkgChannel;
		extern Int8					bkgMusicPath[MAX_BUFFER];

		// Sound effects
		extern std::map<Int8*, FMOD::Sound*> sfxSound;
		extern std::map<Int8*, FMOD::Channel*> sfxChannel;
	}
}

#endif // __PAPYRUS_SOUND_H__