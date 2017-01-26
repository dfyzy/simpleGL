#ifndef SIMPLE_SHADER_DATA_H
#define SIMPLE_SHADER_DATA_H

#include <string>

namespace simpleShaderData {

	std::string getVertex();
	std::string getDefaultGeometry();
	std::string getLightingGeometry();

	std::string getDefaultFragment();
	std::string getEmptyFragment();
	std::string getTextFragment();

	std::string getLightingDefaultFragment();
	std::string getLightingPow2Fragment();

	std::string getOverlayVertex();
	std::string getOverlayFragment();

}

#endif
