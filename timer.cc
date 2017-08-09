#include "timer.h"
#include "util.h"

namespace simpleGL {

std::list<AbstractTimer*> AbstractTimer::timers;
bool AbstractTimer::firstConst = true;

void AbstractTimer::update() {
	for (AbstractTimer* timer : timers)
		timer->step();

}

AbstractTimer::AbstractTimer() {
	if (firstConst) {
		firstConst = false;
		util::addPostUpdate(update);
	}

	timers.push_back(this);
}

}
