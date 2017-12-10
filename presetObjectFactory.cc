#include "presetObjectFactory.h"
#include "util.h"

namespace simpleGL {

PresetObjectFactory* PresetObjectFactory::constructingPresetObjectFactory = nullptr;

PresetObjectFactory::Attribute::Attribute(const Attribute& other) {
	if (constructingPresetObjectFactory) {
		constructingPresetObjectFactory->attributes.push_back(this);

	} else {
		util::println("error::PresetObjectFactoryParam::Trying to construct PresetObjectFactoryParam outside of PresetObjectFactory");
	}
}

}
