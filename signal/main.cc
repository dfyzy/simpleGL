#include <simpleGL/simpleGL.h>
#include <simpleGL/cursor.h>
#include <simpleGL/signal.h>

using namespace simpleGL;

Audio* audio;

class StartButton : public Button {
private:
	Sprite* sprite;

public:
	StartButton(Sprite* sprite) : Button(sprite), sprite(sprite) {}

	virtual void onPress(int mouseButton) {
		//speaker->play();
	}

};

int main() {
	loadWindow("signal", 1000, 700, false, true, {0.3f});

	audio = new Audio();

	Signal signal {48000};
	signal.loadSine(500.0f, 20000);

	signal.loadIntoAudio(audio);

	Speaker* speaker = new Speaker(audio, nullptr, {});
	speaker->setLooping(true);

	speaker->play();

	draw();
}
