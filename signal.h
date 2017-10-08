/* Sound data
*/

#ifndef SIMPLE_SIGNAL_H
#define SIMPLE_SIGNAL_H

#include <memory>
#include <functional>

#include "angle.h"

namespace simpleGL {

class Signal {
private:
	std::unique_ptr<short[]> data;
	unsigned length {0};

	unsigned sampleRate;

public:
	Signal(unsigned sampleRate) : sampleRate(sampleRate) {}
	Signal(unsigned sampleRate, unsigned length, short* array) : data(array), length(length), sampleRate(sampleRate) {}

	Signal(const Signal& other) { operator=(other); }

	const short* getData() const { return data.get(); }

	unsigned getLength() const { return length; }
	unsigned getSampleRate() const { return sampleRate; }

	Signal& loadFunc(float duration, unsigned frequency, std::function<short(float)> func) {
		if (frequency == 0 || sampleRate == 0)	return *this;

		unsigned one = sampleRate/frequency;
		unsigned freqFunc = std::ceil(duration*frequency);

		length = freqFunc*one;

		data.reset(new short[length]);
		for (unsigned k = 0; k < freqFunc; k++)
			for (unsigned i = 0; i < one; i++)
				data[one*k + i] = func(((float) i)/one);

		return *this;
	}

	Signal& loadSine(float duration, unsigned frequency, short amplitude) {
		return loadFunc(duration, frequency, [=] (float t) { return amplitude*std::sin(2.0f*PI*t); });
	}

	Signal& loadSquare(float duration, unsigned frequency, short amplitude) {
		return loadFunc(duration, frequency, [=] (float t) { return std::copysign(amplitude, 0.5f - t); });
	}

	Signal& loadTriangle(float duration, unsigned frequency, short amplitude) {
		return loadFunc(duration, frequency, [=] (float t) { return amplitude*(1.0f - 4.0f*std::abs(t - std::floor(t + 0.25f) - 0.25f)); });
	}

	Signal& loadSawtooth(float duration, unsigned frequency, short amplitude) {
		return loadFunc(duration, frequency, [=] (float t) { return 2.0f*amplitude*(t - std::floor(t + 0.5f)); });
	}

	Signal& loadInverseSawtooth(float duration, unsigned frequency, short amplitude) {
		return loadFunc(duration, frequency, [=] (float t) { return amplitude*(1.0f - t/PI); });//only works in [0, 2*PI]
	}

	void loadIntoSound(class Sound* sound) const;

	//TODO
	//void loadIntoImage(Image* image, unsigned width, unsigned height) const

	void unloadData() {
		data.reset();
		length = 0;
	}

	Signal& operator=(const Signal& other) {
		length = other.length;
		sampleRate = other.sampleRate;

		data.reset(new short[length]);
		for (unsigned i = 0; i < length; i++)
			data[i] = other.data[i];
	}

};

}

#endif
