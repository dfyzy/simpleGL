#include "speaker.h"
#include "util.h"

namespace simpleGL {

Speaker::Speaker(Audio* audio, Point* parent, Vector position, Vector scale, Angle rotation)
													: Point(parent, position, scale, rotation), audio(audio) {
	util::println("Speaker:load");

	alGenSources(1, &id);

	alSourcei(id, AL_BUFFER, audio->getId());

}

Speaker::~Speaker() {
	util::println("Speaker:unload");

	alDeleteSources(1, &id);
}

}
