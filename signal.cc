#include "signal.h"
#include "sound.h"

namespace simpleGL {

void Signal::loadIntoSound(Sound* sound) const {
	if (!sound)	return;

	sound->loadData(1, sampleRate, length, data.get());
}

}
