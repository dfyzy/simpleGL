#include "streamSpeaker.h"
#include "util.h"

namespace simpleGL {

std::list<StreamSpeaker*> StreamSpeaker::streams;
bool StreamSpeaker::firstConst = true;

void StreamSpeaker::update() {
	for (StreamSpeaker* stream : streams)
		stream->step();

}

StreamSpeaker::StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, int bufferSize)
	: Speaker(parent, position, scale, rotation), bufferSize(bufferSize), sounds(new Sound*[bufferSize]) {
	if (firstConst) {
		firstConst = false;
		util::addPostUpdate(update);
	}

	for (int i = 0; i < bufferSize; i++)
		sounds[i] = new Sound();

	streams.push_back(this);
}

void StreamSpeaker::bindData(int i) {
	if (!streaming) return;

	if (getData(sounds[i])) {
		ALuint buffer = sounds[i]->getId();
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

void StreamSpeaker::step() {
	ALint stopped = getState() == AL_STOPPED;

	if (streaming) {
		ALint processed;
		alGetSourcei(getId(), AL_BUFFERS_PROCESSED, &processed);

		while (processed--) {
			ALuint buff;
			alSourceUnqueueBuffers(getId(), 1, &buff);
			if (buff != sounds[index]->getId())	{
				util::println("error:StreamSpeaker:welp");//temp
				streaming = false;
			}

			bindData(index);

			if (++index == bufferSize)	index = 0;
		}

		if (stopped) {
			alSourcePlay(getId());
			util::println("buffering");//temp
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
