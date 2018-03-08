#include "sortedFrame.h"
#include "util.h"

namespace simpleGL {

SortedFrame::~SortedFrame() {
	util::unloadContainer(sprites);
}

}
