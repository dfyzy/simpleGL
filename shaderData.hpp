#ifndef SIMPLE_SHADER_DATA_H
#define SIMPLE_SHADER_DATA_H

#include <string>

namespace simpleShaderData {

	std::string getVertex();
	std::string getGeometry();

	std::string getDefaultFragment();
	std::string getEmptyFragment();
	std::string getTextFragment();
	std::string getLightFragment();

	std::string getOverlayVertex();
	std::string getOverlayFragment();

}

#endif
