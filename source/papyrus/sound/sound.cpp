
// Library Includes
#include <fmod_errors.h>

// Local Includes
#include "../logging/logger.h"
#include "sound.h"

using namespace Papyrus;

FMOD::System*		Sound::system = 0;

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

	FMOD::Sound* sound = 0;
	FMOD::Channel* channel = 0;
	result = system->createSound("data/audio/music/titleScreen.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);

	if (FMOD_OK != result)
	{
		Logger::WriteToFile("FMOD system->createSound - %s: %d - %s", "data/audio/music/titleScreen.mp3", result, FMOD_ErrorString(result));
		return false;
	}

	result = system->playSound(sound, 0, false, &channel);
	
	return true;
}

Bool Sound::ShutDown()
{
	return true;
}