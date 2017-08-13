/* Sound data
*/

#ifndef SIMPLE_SIGNAL_H
#define SIMPLE_SIGNAL_H

#include <memory>
#include <functional>

#include "angle.h"
#include "image.h"
#include "audio.h"

namespace simpleGL {

class Signal {
private:
	std::unique_ptr<short[]> data;
	unsigned length {0};

	const unsigned sampleRate;

public:
	Signal(unsigned sampleRate) : sampleRate(sampleRate) {}
	Signal(unsigned sampleRate, unsigned length, short* array) : data(array), length(length), sampleRate(sampleRate) {}

	const short* getData() const { return data.get(); }

	unsigned getLength() const { return length; }
	unsigned getSampleRate() const { return sampleRate; }

	Signal& loadFunc(float frequency, std::function<short(float)> func) {
		length = sampleRate/frequency;
		data.reset(new short[length]);
		for (unsigned i = 0; i < length; i++)
			data[i] = func((2.0f*PI*i)/length);

		return *this;
	}

	Signal& loadSine(float frequency, short amplitude) {
		return loadFunc(frequency, [=] (float t) { return amplitude*std::sin(t); });
	}

	Signal& loadSquare(float frequency, short amplitude) {
		return loadFunc(frequency, [=] (float t) { return std::copysign(amplitude, PI - t); });//only works in [0, 2*PI]
	}

	Signal& loadTriangle(float frequency, short amplitude) {
		return loadFunc(frequency,
			[=] (float t) {
				t = t/(2.0f*PI) + 0.25f;
				return amplitude*(1.0f - 4.0f*std::abs(t - std::floor(t) - 0.5f));
			}
		);
	}

	Signal& loadSawtooth(float frequency, short amplitude) {
		return loadFunc(frequency,
			[=] (float t) {
				t = t/(2.0f*PI) + 0.5f;
				return amplitude*(2.0f*(t - std::floor(t)) - 1.0f);
			}
		);
	}

	Signal& loadInverseSawtooth(float frequency, short amplitude) {
		return loadFunc(frequency, [=] (float t) { return amplitude*(1.0f - t/PI); });
	}

	void loadIntoAudio(Audio* audio) const {
		if (!audio)	return;

		audio->loadData(1, length, sampleRate, data.get());
	}

	void loadIntoImage(Image* image, unsigned width, unsigned height) const {
		if (!image)	return;

		//TODO
	}

	void unloadData() {
		data.reset();
		length = 0;
	}

};

}

#endif
