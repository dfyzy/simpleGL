#include "speaker.h"
#include "util.h"

namespace simpleGL {

Speaker::Speaker(Audio* audio, Point* parent, Vector position, Vector scale, Angle rotation)
													: Point(parent, position, scale, rotation), audio(audio) {
	util::println("Speaker:load");

	alGenSources(1, &id);

	if (audio)	audio->addSpeaker(this);
}

Speaker::~Speaker() {
	util::println("Speaker:unload");

	alDeleteSources(1, &id);

	if (audio)	audio->removeSpeaker(this);
}

}
