/* Speaker for playing streams of sounds
*/

#ifndef SIMPLE_STREAM_SPEAKER_H
#define SIMPLE_STREAM_SPEAKER_H

#include <memory>

#include "speaker.h"
#include "updatable.h"

namespace simpleGL {

class Sound;

class StreamSpeaker : public Speaker, public Updatable<EUpdateType::PostTick> {
private:
	const int bufferSize;

	std::unique_ptr<Sound[]> sounds;
	int index {0};

	bool streaming {false};
	bool bound {false};

	bool canLoop {true};

	void update() override;

	void bindData(int i);

protected:
	~StreamSpeaker();

	virtual void openStream() =0;
	virtual bool getData(Sound& sound) =0;
	virtual void restartStream() =0;
	virtual void closeStream() =0;

public:
	StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, int bufferSize);

	void play() override;

	void stop() override;

};

}

#endif
