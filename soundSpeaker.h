/* Speaker for playing single sounds
*/

#ifndef SIMPLE_SOUND_SPEAKER_H
#define SIMPLE_SOUND_SPEAKER_H

#include "speaker.h"

namespace simpleGL {

class Sound;

class SoundSpeaker : public Speaker {
private:
	Sound* sound {nullptr};

protected:
	~SoundSpeaker() {
		stop();
		alSourcei(getId(), AL_BUFFER, 0);
	}

public:
	SoundSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, Sound* sound) : Speaker(parent, position, scale, rotation) {
		setSound(sound);
	}

	Sound* getSound() const { return sound; }
	void setSound(Sound* s);

	void setLooping(bool b) override {
		Speaker::setLooping(b);
		alSourcei(getId(), AL_LOOPING, b ? AL_TRUE : AL_FALSE);
	}

	void play() override {
		alSourcePlay(getId());
	}

	void stop() override {
		alSourceStop(getId());
	}

};

}

#endif
