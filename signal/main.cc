#include <simpleGL/simpleGL.h>
#include <simpleGL/slider.h>
#include <simpleGL/signal.h>
#include <simpleGL/math.h>

using namespace simpleGL;

Audio* audio;
Signal* sig;

constexpr float MIN_FREQ = 100.0f;
constexpr float MAX_FREQ = 10000.0f;

constexpr short MIN_AMPL = 100;
constexpr short MAX_AMPL = 30000;

float freq = math::lerp(MIN_FREQ, MAX_FREQ, 0.5f);
short ampl = math::lerp(MIN_AMPL, MAX_AMPL, 0.5f);

void updateSig() {
	sig->loadSine(freq, ampl);
	sig->loadIntoAudio(audio);
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

	audio = new Audio();

	sig = new Signal(48000);
	updateSig();

	Angle hpi {PI/2};
	Slider* freqSL = Slider::Loader().texture({50}).color({0.5f, 0, 0}).position({-100, 0}).length(200).slideRotation(hpi).load();
	freqSL->setCallback(freqSlider);

	Slider* amplSL = Slider::Loader().texture({50}).color({0, 0.5f, 0}).position({100, 0}).length(200).slideRotation(hpi).load();
	amplSL->setCallback(amplSlider);

	Speaker* speaker = new Speaker(audio, nullptr, {});
	speaker->setLooping(true);

	speaker->play();

	draw();
}
