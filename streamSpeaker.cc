#include "streamSpeaker.h"
#include "sound.h"
#include "util.h"

namespace simpleGL {

StreamSpeaker::StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, int bufferSize)
	: Speaker(parent, position, scale, rotation), bufferSize(bufferSize), sounds(new Sound[bufferSize]) {
}

StreamSpeaker::~StreamSpeaker() {
	stop();
}

void StreamSpeaker::bindData(int i) {
	if (!streaming) return;

	if (getData(sounds[i])) {
		ALuint buffer = sounds[i].getId();
		alSourceQueueBuffers(getId(), 1, &buffer);

	} else {
		if (getLooping() && canLoop) {
			restartStream();
			canLoop = false;
			bindData(i);
			canLoop = true;
		} else	streaming = false;
	}
}

void StreamSpeaker::update() {
	ALint stopped = getState() == AL_STOPPED;

	if (streaming) {
		ALint processed;
		alGetSourcei(getId(), AL_BUFFERS_PROCESSED, &processed);

		while (processed--) {
			ALuint buff;
			alSourceUnqueueBuffers(getId(), 1, &buff);
			if (buff != sounds[index].getId())	{
				util::println("error:StreamSpeaker:welp");//temp
				streaming = false;
			}

			bindData(index);

			if (++index == bufferSize)	index = 0;
		}

		if (stopped) {
			alSourcePlay(getId());
			util::println("warning:StreamSpeaker:stream stopped before it should, maybe you should increase buffersize or length of signals");
		}
	} else if (stopped && bound) {
		alSourcei(getId(), AL_BUFFER, 0);
		bound = false;
		closeStream();
	}
}

void StreamSpeaker::play() {
	ALint state = getState();

	if (state != AL_PAUSED) {

		if (state == AL_PLAYING) {
			alSourceStop(getId());
			alSourcei(getId(), AL_BUFFER, 0);

			restartStream();
		} else	openStream();

		streaming = true;
		index = 0;

		for (int i = 0; i < bufferSize; i++)	bindData(i);

		bound = true;
	}

	alSourcePlay(getId());
}

void StreamSpeaker::stop() {
	alSourceStop(getId());
	streaming = false;
	alSourcei(getId(), AL_BUFFER, 0);
	bound = false;
	closeStream();
}

}
