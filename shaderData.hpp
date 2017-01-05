#ifndef SIMPLE_SHADER_DATA_H
#define SIMPLE_SHADER_DATA_H

#include <string>

namespace simpleShaderData {

	extern std::string getVertex();
	extern std::string getGeometry();

	extern std::string getDefaultFragment();
	extern std::string getEmptyFragment();
	extern std::string getTextFragment();

	extern std::string getOverlayVertex();
	extern std::string getOverlayFragment();

}

#endif
