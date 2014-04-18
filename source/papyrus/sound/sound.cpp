
// Library Includes
#include <fmod_errors.h>

// Local Includes
#include "../logging/logger.h"
#include "sound.h"

using namespace Papyrus;

FMOD::System*		Sound::system = 0;
FMOD::Sound*		Sound::bkgMusic = 0;
FMOD::Channel*		Sound::bkgChannel = 0;
Int8				Sound::bkgMusicPath[MAX_BUFFER];

FMOD::Sound*		Sound::sfxMusic;
FMOD::Channel*		Sound::sfxChannel;
Int8				Sound::sfxMusicPath[MAX_BUFFER];

#define PY_FMOD_RELEASE(Object) if (0 != Object) { Object->release(); Object = 0; }
#define PY_FMOD_CLOSE(Object) if (0 != Object) { Object->close(); }

Bool Sound::Initialise() 
{
	FMOD_RESULT result;
	
	result = FMOD::System_Create(&system);

	if (FMOD_OK != result)
	{
		Logger::WriteToFile("FMOD System_Create failed: %d - %s", result, FMOD_ErrorString(result));
		return false;
	}

	UInt32 version = 0;
	result = system->getVersion(&version);

	if (version < FMOD_VERSION)
	{
		Logger::Write("FMOD lib version doesn't match, version %08x doesn't match header version %08x", version, FMOD_VERSION);
		return false;
	}

	void* extraDriverData = 0;

	result = system->init(32, FMOD_INIT_NORMAL, extraDriverData);	

	if (FMOD_OK != result)
	{
		Logger::WriteToFile("FMOD system->init failed: %d - %s", result, FMOD_ErrorString(result));
		return false;
	}
	
	return true;
}

Bool Sound::ShutDown()
{
	PY_FMOD_RELEASE(bkgMusic);
	PY_FMOD_CLOSE(system);
	PY_FMOD_RELEASE(system);

	return true;
}

Bool Sound::PlayBkgMusic(Int8* _path)
{
	if (!SDL_strcmp(_path, bkgMusicPath))
	{
		// already playing, return out
		return true;
	}

	FMOD_RESULT result;

	if (0 != bkgChannel)
	{
		PY_FMOD_RELEASE(bkgMusic);
		bkgChannel = 0;
	}

	result = system->createSound(_path, FMOD_LOOP_NORMAL | FMOD_2D, 0, &bkgMusic);

	if (FMOD_OK != result)
	{
		Logger::WriteToFile("FMOD system->createSound - %s: %d - %s", _path, result, FMOD_ErrorString(result));
		return false;
	}
	
	result = system->playSound(bkgMusic, 0, false, &bkgChannel);

	if (FMOD_OK != result)
	{
		Logger::WriteToFile("FMOD system->playSound - %s: %d - %s", _path, result, FMOD_ErrorString(result));
		return false;
	}

	SDL_snprintf(bkgMusicPath, MAX_BUFFER, "%s", _path);

	return true;
}

Bool Sound::PlaySFX(Int8* _path)
{
	return true;
}