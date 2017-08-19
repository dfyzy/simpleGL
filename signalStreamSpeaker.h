/* StreamSpeaker for continuous signals
*/

#ifndef SIMPLE_SIGNAL_STREAM_SPEAKER_H
#define SIMPLE_SIGNAL_STREAM_SPEAKER_H

#include "streamSpeaker.h"
#include "signal.h"

namespace simpleGL {

class SignalStreamSpeaker : public StreamSpeaker {
private:
	Signal signal;

protected:
	~SignalStreamSpeaker() {}

	void openStream() override {}
	bool getData(Sound* sound) override {
		signal.loadIntoSound(sound);

		return true;
	}
	void restartStream() override {}
	void closeStream() override {}

public:
	SignalStreamSpeaker(Point* parent, Vector position, Vector scale, Angle rotation, const Signal& signal)
		: StreamSpeaker(parent, position, scale, rotation, 2), signal(signal) {}

	void setSignal(const Signal& s) { signal = s; }

};

}

#endif
