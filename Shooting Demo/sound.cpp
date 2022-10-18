#include "sound.h"

irrklang::ISoundEngine* Sound::sound_engine = irrklang::createIrrKlangDevice();

void Sound::play2D(const char* sound_file_name, bool play_looped, bool start_paused, bool track) {
	sound_engine->play2D(sound_file_name, play_looped, start_paused, track);
}

