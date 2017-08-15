/* Abstract wrapper class for OpenAL source object
 * Has some positional stuff(or will have)
*/

#ifndef SIMPLE_SPEAKER_H
#define SIMPLE_SPEAKER_H

#include "sound.h"
#include "point.h"

namespace simpleGL {

class Speaker : public Point {
private:
	ALuint id;

protected:
	~Speaker() {
		alDeleteSources(1, &id);
	}

public:
	Speaker(Point* parent, Vector position, Vector scale, Angle rotation) : Point(parent, position, scale, rotation) {
		alGenSources(1, &id);
	}

	ALuint getId() const { return id; }

	ALint getState() const {
		ALint result;
		alGetSourcei(getId(), AL_SOURCE_STATE, &result);
		return result;
	}

	//TODO: looping

	virtual void play() =0;
	//TODO
	void pause() {
		alSourcePause(id);
	}

	virtual void stop() =0;

};

}

#endif
