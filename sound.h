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

	int channels {0};
	unsigned sampleRate {0};
	unsigned length {0};

public:
	Sound();

	Sound(const Sound& other) =delete;

	~Sound();

	void loadData(int pchannels, unsigned psampleRate, unsigned plength, const short* data);
	void loadData(const std::string& path);

	ALuint getId() const { return id; }

	int getChannels() const { return channels; }
	unsigned getSampleRate() const { return sampleRate; }
	unsigned getLength() const { return length; }

};

}

#endif
