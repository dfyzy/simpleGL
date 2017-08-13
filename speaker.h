/* A wrapper around OpenAL speaker object
*/

#ifndef SIMPLE_SPEAKER_H
#define SIMPLE_SPEAKER_H

#include "audio.h"
#include "point.h"

namespace simpleGL {

class Speaker : public Point {
private:
	ALuint id;

	Audio* audio;

	bool playing {false};
	bool looping {false};

	ALint pausePos {0};

protected:
	~Speaker();

public:
	Speaker(Audio* audio, Point* parent, Vector position, Vector scale, Angle rotation);
	Speaker(Audio* audio, Point* parent, Vector position) : Speaker(audio, parent, position, {1}, {}) {}

	ALuint getId() const { return id; }

	Audio* getAudio() const { return audio; }
	void setAudio(Audio* a) {
		bool pl = playing;
		if (pl)	stop();

		if (audio)	audio->removeSpeaker(this);

		audio = a;

		if (audio)	audio->addSpeaker(this);

		if (pl)	play();
	}

	bool isPlaying() const { return playing; }

	bool isLooping() const { return looping; }
	void setLooping(bool b) {
		looping = b;
		alSourcei(id, AL_LOOPING, b ? AL_TRUE : AL_FALSE);
	}

	ALint getPausePos() const { return pausePos; }
	void setPausePos(ALint i) { pausePos = i; }

	void play() {
		if (!audio)	return;

		if (pausePos > 0)	alSourcei(id, AL_SAMPLE_OFFSET, pausePos);

		alSourcei(id, AL_BUFFER, audio->getId());
		alSourcePlay(id);

		pausePos = 0;
		playing = true;
	}
	void pause() {
		if (playing) {
			alGetSourcei(id, AL_SAMPLE_OFFSET, &pausePos);
			alSourceStop(id);

			alSourcei(id, AL_BUFFER, 0);
		}

		playing = false;
	}
	void stop() {
		if (playing) {
			alSourceStop(id);
			alSourcei(id, AL_BUFFER, 0);
		}

		pausePos = 0;
		playing = false;
	}

	void unload() { delete this; }

};

}

#endif
