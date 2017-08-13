/* A wrapper around OpenAL buffer object
*/

#ifndef SIMPLE_AUDIO_H
#define SIMPLE_AUDIO_H

#include <AL/alext.h>
#include <string>
#include <list>

namespace simpleGL {

class Speaker;

class Audio {
private:
	ALuint id {0};

	unsigned channels {0};
	unsigned length {0};
	unsigned sampleRate {0};

	std::list<Speaker*> speakers;

protected:
	~Audio();

public:
	Audio();

	Audio* loadData(unsigned pchanels, unsigned plength, unsigned psampleRate, const short* data);
	Audio* loadData(std::string path);

	ALuint getId() const { return id; }

	unsigned getChannels() const { return channels; }
	unsigned getLength() const { return length; }
	unsigned getSampleRate() const { return sampleRate; }

	void addSpeaker(Speaker* s) { speakers.push_back(s); }
	void removeSpeaker(Speaker* s) { speakers.remove(s); }

	void unload() { delete this; }

};

}

#endif
