#include <simpleGL/simpleGL.h>
#include <simpleGL/signalStreamSpeaker.h>
#include <simpleGL/slider.h>
#include <simpleGL/signal.h>
#include <simpleGL/math.h>

using namespace simpleGL;

constexpr unsigned MIN_FREQ = 100;
constexpr unsigned MAX_FREQ = 3000;

constexpr short MIN_AMPL = 100;
constexpr short MAX_AMPL = 10000;

Signal* sig;
SignalStreamSpeaker* speaker;

unsigned freq = math::lerp(MIN_FREQ, MAX_FREQ, 0.5f);
short ampl = math::lerp(MIN_AMPL, MAX_AMPL, 0.5f);

void updateSig() {
	sig->loadSine(0.03f, freq, ampl);
	speaker->setSignal(*sig);
}

void freqSlider(float f) {
	freq = math::lerp(MIN_FREQ, MAX_FREQ, f);
	updateSig();
}

void amplSlider(float f) {
	ampl = math::lerp(MIN_AMPL, MAX_AMPL, f);
	updateSig();
}

int main() {
	loadWindow("signal", 1000, 700, false, true, {0.3f});

	sig = new Signal(48000);

	Angle hpi {PI/2};
	Slider* freqSL = Slider::Loader().texture({50}).color({0.5f, 0, 0}).position({-100, 0}).length(200).slideRotation(hpi).load();
	freqSL->setCallback(freqSlider);

	Slider* amplSL = Slider::Loader().texture({50}).color({0, 0.5f, 0}).position({100, 0}).length(200).slideRotation(hpi).load();
	amplSL->setCallback(amplSlider);

	// speaker = new FileStreamSpeaker(nullptr, {}, {1}, {}, "signal/takenobu.flac");
	speaker = new SignalStreamSpeaker(nullptr, {}, {1}, {}, *sig);
	updateSig();
	speaker->play();

	draw();
}
