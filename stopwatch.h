#ifndef SIMPLE_STOPWATCH_H
#define SIMPLE_STOPWATCH_H

#include <chrono>

namespace simpleGL {

class Stopwatch {
private:
	typedef std::chrono::high_resolution_clock Clock;

	Clock::time_point point;

	double diffInSeconds(Clock::time_point other) const {
		return std::chrono::duration_cast<std::chrono::microseconds>(other - point).count() * 0.000001;
	}

public:
	void start() {
		point = Clock::now();
	}

	double get() const {
		return diffInSeconds(Clock::now());
	}

	double lap() {
		Clock::time_point now = Clock::now();
		double result = diffInSeconds(now);

		point = now;
		return result;
	}

};

}

#endif
