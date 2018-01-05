#include "signal.h"
#include "sound.h"

namespace simpleGL {

void Signal::loadIntoSound(Sound& sound) const {
	sound.loadData(1, sampleRate, length, data.get());
}

}
