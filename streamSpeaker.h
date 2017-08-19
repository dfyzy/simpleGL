/* Speaker for playing streams of sounds
*/

#ifndef SIMPLE_STREAM_SPEAKER_H
#define SIMPLE_STREAM_SPEAKER_H

#include <memory>

#include "sound.h"
#include "speaker.h"

namespace simpleGL {


class StreamSpeaker : public Speaker {
private:
	static std::list<simpleGL::StreamSpeaker*> streams;
	static bool firstConst;
	static void update();

	const int bufferSize;

	std::unique_ptr<Sound*[]> sounds;
	int index {0};

	bool streaming {false};
	bool bound {false};

	bool canLoop {true};

	void step();

	void bindData(int i);

protected:
	~StreamSpeaker() {
		stop();

		for (int i = 0; i < bufferSize; i++)
			sounds[i]->unload();

		streams.remove(this);
	}

	virtual void openStream() =0;
	virtual bool getData(Sound* sound) =0;
	virtual void restartStream() =0;
	virtual void closeStream() =0;

public:
	StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, int bufferSize);

	void play() override;

	void stop() override;

};

}

#endif
