/* Some shader sources
*/

#ifndef SIMPLE_SHADER_DATA_H
#define SIMPLE_SHADER_DATA_H

#include <string>

namespace simpleGL {

namespace shaderData {

std::string getVertex();

std::string getDefaultFragment();
std::string getEmptyFragment();
std::string getTextFragment();

std::string getOverlayVertex();
std::string getOverlayFragment();

std::string getLightingDefaultFragment();
std::string getLightingPow2Fragment();

}

}

#endif
