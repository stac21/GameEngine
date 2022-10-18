#pragma once

#include <string>
#include <irrKlang\irrKlang.h>

class Sound {
private:
	static irrklang::ISoundEngine* sound_engine;

public:
	static void play2D(const char* sound_file_name, bool play_looped = false, bool start_paused = false, bool track = false);
	static void play3D(const char* sound_file_name);
};