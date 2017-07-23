#ifndef SIMPLE_SPEAKER_H
#define SIMPLE_SPEAKER_H

#include "audio.h"
#include "point.h"

namespace simpleGL {

class Speaker : public Point {
private:
	ALuint id;

	Audio* audio;

protected:
	~Speaker();

public:
	Speaker(Audio* audio, Point* parent, Vector position, Vector scale, Angle rotation);
	Speaker(Audio* audio, Point* parent, Vector position) : Speaker(audio, parent, position, {1}, {}) {}

	ALuint getId() const { return id; }

	void play() { alSourcePlay(id); }

	void pause() { alSourcePause(id); }//Doesn't work, at least on my system.

	void stop() { alSourceStop(id); }

	void unload() { delete this; }

};

}

#endif
