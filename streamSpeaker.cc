#include "streamSpeaker.h"
#include "util.h"

namespace simpleGL {

std::list<StreamSpeaker*> StreamSpeaker::streams;
bool StreamSpeaker::firstConst = true;

void StreamSpeaker::update() {
	for (StreamSpeaker* stream : streams)
		stream->step();

}

StreamSpeaker::StreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation) : Speaker(parent, position, scale, rotation) {
	if (firstConst) {
		firstConst = false;
		util::addPostUpdate(update);
	}

	for (int i = 0; i < BUFFER_SIZE; i++)
		sounds[i] = new Sound();

	streams.push_back(this);
}

void StreamSpeaker::step() {
	if (streaming) {
		//check if stopped?

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

			if (++index == BUFFER_SIZE)	index = 0;
		}
	} else if (bound) {
		//check if stopped!

		alSourcei(getId(), AL_BUFFER, 0);
		bound = false;
		closeStream();//error above
	}
}

}
