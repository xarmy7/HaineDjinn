#include "Game/SoundSystem.h"
#include "Core/Debug.h"

namespace SoundSystem
{
	void Init()
	{
		engine = irrklang::createIrrKlangDevice();

		if (engine == nullptr)
			HD_LOG("Sound can't be played, because no sound devices has been detected", LogSeverity::LOG_ERROR);
	}

	void PlaySound2D(const char* path, bool loop)
	{
		// play some sound stream, looped or not
		if (engine != nullptr)
			engine->play2D(path, loop);
	}

	void PlaySound3D(const char* path, Vector3 position, bool loop)
	{
		irrklang::vec3df conv;
		conv.X = position.x;
		conv.Y = position.y;
		conv.Z = position.z;

		// play some sound stream in a position, looped or not
		if (engine != nullptr)
			engine->play3D(path, conv, loop);
	}
}