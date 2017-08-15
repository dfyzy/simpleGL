/* As it says on the title
*/

#ifndef SIMPLE_FILE_STREAM_SPEAKER_H
#define SIMPLE_FILE_STREAM_SPEAKER_H

#include "streamSpeaker.h"
#include "soundFile.h"

namespace simpleGL {

class FileStreamSpeaker : public StreamSpeaker {
private:
	SoundFile* file {nullptr};
	std::string path;

protected:
	~FileStreamSpeaker() {}

	void openStream() override {
		file = new SoundFile(path);

	}

	bool getData(Sound* sound) override {
		return file->read(sound, 4000);
	}

	void closeStream() override {
		delete file;
	}

public:
	FileStreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, std::string path)
		: StreamSpeaker(parent, position, scale, rotation), path(path) {}

};

}

#endif
