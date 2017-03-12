#include "shaderData.h"

namespace simpleShaderData {

	std::string getVertex() {
		return std::string("#version 430 core\n"
		"layout(location=0)	in vec2 inPosition;\n"
		"layout(location=1)	in vec2 inTexture;\n"

		"out vec4 vColor;\n"
		"out vec2 vTexture;\n"

		"out gl_PerVertex {\n"
		"  vec4 gl_Position;\n"
		"};\n"

		"layout(std140) uniform DynamicData {\n"
		"	uniform vec4 color;\n"
		"	uniform vec2 resolution;\n"
		"	uniform vec2 cameraPosition;\n"
		"	uniform vec2 cameraScale;\n"
		"	uniform float cameraRotation;\n"
		"} dynamic;\n"

		"void main() {\n"
		"	vTexture = inTexture;\n"
		"	vColor = dynamic.color;\n"

		"	float sinR = sin(dynamic.cameraRotation);\n"
		"	float cosR = cos(dynamic.cameraRotation);\n"
		"	mat2 rot = mat2(cosR,	sinR,"
								"-sinR,	cosR);\n"

		"	gl_Position = vec4(rot * (inPosition - dynamic.cameraPosition), 0, 1);\n"
		"	gl_Position.xy *= dynamic.cameraScale * 2/dynamic.resolution.y;"
		"	gl_Position.x *= dynamic.resolution.y/dynamic.resolution.x;\n"
		"}");
	}

	std::string getDefaultFragment() {
		return std::string("#version 430 core\n"
		"in vec4 vColor;\n"
		"in vec2 vTexture;\n"

		"out vec4 fColor;\n"

		"uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = texture(text, vTexture) * vColor;\n"
		"}");
	}

	std::string getEmptyFragment() {
		return std::string("#version 430 core\n"
		"in vec4 vColor;\n"
		"in vec2 vTexture;\n"

		"out vec4 fColor;\n"

		"void main() {\n"
		"	fColor = vColor;\n"
		"}");
	}

	std::string getTextFragment() {
		return std::string("#version 430 core\n"

		"in vec4 vColor;\n"
		"in vec2 vTexture;\n"

		"out vec4 fColor;\n"

		"uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = vColor;\n"
		"	fColor.a *= texture(text, vTexture).r;\n"
		"}");
	}

	std::string getOverlayVertex() {
		return std::string("#version 430 core\n"
		"layout(location=0)	in vec2 inPosition;\n"

		"out gl_PerVertex {\n"
		"  vec4 gl_Position;\n"
		"};\n"

		"void main() {\n"
		"	gl_Position = vec4(inPosition, 0, 1);\n"
		"}");
	}

	std::string getOverlayFragment() {
		return std::string("#version 430 core\n"
		"in vec4 gl_FragCoord;\n"

		"out vec4 fColor;\n"

		"uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = texture(text, gl_FragCoord.xy);\n"
		"}");
	}

	std::string getLightingDefaultFragment() {
		return std::string("#version 430 core\n"

		"in vec4 vColor;\n"
		"in vec2 vTexture;\n"

		"out vec4 fColor;\n"

		"uniform vec2 centre;\n"
		"uniform vec2 bounds;\n"

		"void main() {\n"
		"	fColor = vColor;\n"
		"}");
	}

	std::string getLightingPow2Fragment() {
		return std::string("#version 430 core\n"

		"in vec4 vColor;\n"
		"in vec2 vTexture;\n"

		"out vec4 fColor;\n"

		"uniform vec2 centre;\n"
		"uniform vec2 bounds;\n"

		"void main() {\n"
		"	fColor = (1 - pow(2*length((centre - gl_FragCoord.xy)/bounds), 2))*vColor;\n"
		"}");
	}

}
