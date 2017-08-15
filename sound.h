/* A wrapper around OpenAL buffer object
*/

#ifndef SIMPLE_SOUND_H
#define SIMPLE_SOUND_H

#include <AL/alext.h>
#include <string>

namespace simpleGL {

class Sound {
private:
	ALuint id {0};

	unsigned channels {0};
	unsigned length {0};
	unsigned sampleRate {0};

protected:
	~Sound();

public:
	Sound();

	Sound* loadData(unsigned pchanels, unsigned plength, unsigned psampleRate, const short* data);
	Sound* loadData(std::string path);

	ALuint getId() const { return id; }

	unsigned getChannels() const { return channels; }
	unsigned getLength() const { return length; }
	unsigned getSampleRate() const { return sampleRate; }

	void unload() { delete this; }

};

}

#endif
