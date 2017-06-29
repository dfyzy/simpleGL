#include "shaderData.h"

namespace simpleGL {

namespace shaderData {

std::string getVertex() {
	return std::string("#version 330 core\n"
	"layout(location=0)	in vec2 inPosition;\n"
	"layout(location=1)	in vec2 inTexture;\n"
	"layout(location=2)	in vec4 inColor;\n"

	"out vec2 vPosition;\n"
	"out vec2 vTexture;\n"
	"out vec4 vColor;\n"

	"out gl_PerVertex {\n"
	"  vec4 gl_Position;\n"
	"};\n"

	"layout(std140) uniform DynamicData {\n"
	"	mat3 view;\n"
	"	vec2 resolution;\n"
	"} dynamic;\n"

	"void main() {\n"
	"	vPosition = inPosition;\n"
	"	vTexture = inTexture;\n"
	"	vColor = inColor;\n"

	"	gl_Position = vec4(round(dynamic.view * vec3(inPosition, 1)), 1);\n"
	"	gl_Position.xy /= dynamic.resolution.y/2;\n"
	"	gl_Position.x *= dynamic.resolution.y/dynamic.resolution.x;\n"
	"}");
}

std::string getDefaultFragment() {
	return std::string("#version 330 core\n"

	"in vec2 vPosition;\n"
	"in vec2 vTexture;\n"
	"in vec4 vColor;\n"

	"out vec4 fColor;\n"

	"uniform sampler2DRect text;\n"

	"void main() {\n"
	"	vec4 color = texture(text, vTexture) * vColor;\n"
	"	if (color.a <= 0)	discard;"
	"	fColor = color;\n"
	"}");
}

std::string getEmptyFragment() {
	return std::string("#version 330 core\n"

	"in vec2 vPosition;\n"
	"in vec2 vTexture;\n"
	"in vec4 vColor;\n"

	"out vec4 fColor;\n"

	"void main() {\n"
	"	fColor = vColor;\n"
	"}");
}

std::string getTextFragment() {
	return std::string("#version 330 core\n"

	"in vec2 vPosition;\n"
	"in vec2 vTexture;\n"
	"in vec4 vColor;\n"

	"out vec4 fColor;\n"

	"uniform sampler2DRect text;\n"

	"void main() {\n"
	"	fColor = vColor;\n"
	"	fColor.a *= texture(text, vTexture).r;\n"
	"}");
}

std::string getOverlayVertex() {
	return std::string("#version 330 core\n"
	"layout(location=0)	in vec2 inPosition;\n"

	"out gl_PerVertex {\n"
	"  vec4 gl_Position;\n"
	"};\n"

	"void main() {\n"
	"	gl_Position = vec4(inPosition, 0, 1);\n"
	"}");
}

std::string getOverlayFragment() {
	return std::string("#version 330 core\n"
	"in vec4 gl_FragCoord;\n"

	"out vec4 fColor;\n"

	"uniform sampler2DRect text;\n"

	"void main() {\n"
	"	fColor = texture(text, gl_FragCoord.xy);\n"
	"}");
}

std::string getLightingDefaultFragment() {
	return std::string("#version 330 core\n"

	"in vec2 vPosition;\n"
	"in vec2 vTexture;\n"
	"in vec4 vColor;\n"

	"out vec4 fColor;\n"

	"uniform vec2 centre;\n"
	"uniform vec2 bounds;\n"

	"void main() {\n"
	"	fColor = vColor;\n"
	"}");
}

std::string getLightingPow2Fragment() {
	return std::string("#version 330 core\n"

	"in vec2 vPosition;\n"
	"in vec2 vTexture;\n"
	"in vec4 vColor;\n"

	"out vec4 fColor;\n"

	"uniform vec2 centre;\n"
	"uniform vec2 bounds;\n"

	"void main() {\n"
	"	fColor = (1 - pow(2*length((centre - vPosition)/bounds), 2))*vColor;\n"
	"}");
}

}

}
