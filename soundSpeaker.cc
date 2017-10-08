#include "soundSpeaker.h"
#include "sound.h"

namespace simpleGL {

void SoundSpeaker::setSound(Sound* s) {
	sound = s;

	stop();
	alSourcei(getId(), AL_BUFFER, sound ? sound->getId() : 0);
}

}
