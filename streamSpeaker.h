/* Speaker for playing streams of sounds
*/

#ifndef SIMPLE_STREAM_SPEAKER_H
#define SIMPLE_STREAM_SPEAKER_H

#include "sound.h"
#include "speaker.h"

namespace simpleGL {


class StreamSpeaker : public Speaker {
public:
	static constexpr int BUFFER_SIZE = 3;

private:
	static std::list<simpleGL::StreamSpeaker*> streams;
	static bool firstConst;
	static void update();

	Sound* sounds[BUFFER_SIZE];
	int index {0};

	bool streaming {false};
	bool bound {false};

	void step();

	void bindData(int i) {
		if (streaming && getData(sounds[i])) {
			ALuint buffer = sounds[i]->getId();
			alSourceQueueBuffers(getId(), 1, &buffer);

		} else streaming = false;
	}

protected:
	~StreamSpeaker() {
		stop();

		for (int i = 0; i < BUFFER_SIZE; i++)
			sounds[i]->unload();

		streams.remove(this);
	}

	virtual void openStream() =0;
	virtual bool getData(Sound* sound) =0;
	virtual void closeStream() =0;

public:
	StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation);

	void play() override {
		ALint state = getState();

		if (state == AL_PLAYING) stop();

		if (state != AL_PAUSED) {
			index = 0;
			streaming = true;
			openStream();

			for (int i = 0; i < BUFFER_SIZE; i++)	bindData(i);

			bound = true;
		}

		alSourcePlay(getId());
	}

	void stop() override {
		alSourceStop(getId());
		alSourcei(getId(), AL_BUFFER, 0);
		bound = false;
		streaming = false;
		closeStream();
	}

};

}

#endif
