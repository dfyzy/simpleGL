#include "shaderData.hpp"

namespace simpleShaderData {

	std::string getVertex() {
		return std::string("#version 430 core\n"
		"layout(location=1)	in vec2 inPosition;\n"
		"layout(location=2)	in vec2 inBounds;\n"
		"layout(location=3)	in float inAngle;\n"
		"layout(location=4)	in vec4 inColor;\n"
		"layout(location=5)	in vec4 inTexData;\n"

		"out QuadData {\n"
		"	vec2 vBounds;\n"
		"	float vAngle;\n"
		"	vec4 vColor;\n"
		"	vec4 vTexData;\n"
		"};\n"

		"out gl_PerVertex {\n"
		"  vec4 gl_Position;\n"
		"};\n"

		"void main() {\n"
		"	vBounds = inBounds;\n"
		"	vAngle = inAngle;\n"
		"	vColor = inColor;\n"
		"	vTexData = inTexData;\n"
		"	gl_Position = vec4(inPosition, 0, 1);\n"
		"}");
	}

	std::string getGeometry() {
		return std::string("#version 430 core\n"

		"layout(points) in;\n"
		"layout(triangle_strip, max_vertices = 4) out;\n"

		"in gl_PerVertex {\n"
		"  vec4 gl_Position;\n"
		"} gl_in[];\n"

		"in QuadData {\n"
		"	vec2 vBounds;\n"
		"	float vAngle;\n"
		"	vec4 vColor;\n"
		"	vec4 vTexData;\n"
		"} inData[];\n"

		"out gl_PerVertex {\n"
		"	vec4 gl_Position;\n"
		"};\n"

		"out vec4 geomColor;\n"
		"out vec2 geomTexPosition;\n"

		"layout(std140) uniform StaticData {\n"
		"	uniform float rAspect;\n"
		"} stData;\n"

		"layout(std140) uniform DynamicData {\n"
		"	uniform vec2 cameraPosition;\n"
		"	uniform float cameraRotation;\n"
		"} dynData;\n"

		"mat3 rotV, rotC;\n"

		"void drawVert(vec2 vert) {\n"
		"	geomColor = inData[0].vColor;\n"
		"	geomTexPosition = vert * inData[0].vTexData.zw + inData[0].vTexData.xy;//hmmmm\n"

		"	gl_Position = vec4(rotC*(rotV*vec3(vert * inData[0].vBounds, 0) + vec3(gl_in[0].gl_Position.xy - dynData.cameraPosition, 0)), 1);\n"
		"	gl_Position.x *= stData.rAspect;\n"
		"	EmitVertex();\n"
		"}\n"

		"void main() {\n"
		"	float cosV = cos(inData[0].vAngle);\n"
		"	float sinV = sin(inData[0].vAngle);\n"
		"	rotV = mat3(	cosV,		sinV,	0,\n"
		"							-sinV,	cosV,	0,\n"
		"							0,			0,		1);\n"

		"	float cosC = cos(-dynData.cameraRotation);\n"
		"	float sinC = sin(-dynData.cameraRotation);\n"
		"	rotC = mat3(	cosC,		sinC,	0,\n"
		"							-sinC,	cosC,	0,\n"
		"							0,			0,		1);\n"

		"	drawVert(vec2(-0.5, 0.5));\n"
		"	drawVert(vec2(-0.5, -0.5));\n"
		"	drawVert(vec2(0.5, 0.5));\n"
		"	drawVert(vec2(0.5, -0.5));\n"

		"	EndPrimitive();\n"
		"}");
	}

	std::string getDefaultFragment() {
		return std::string("#version 430 core\n"

		"in vec4 geomColor;\n"
		"in vec2 geomTexPosition;\n"

		"out vec4 fColor;\n"

		"uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = texture(text, geomTexPosition) * geomColor;\n"
		"}");
	}

	std::string getEmptyFragment() {
		return std::string("#version 430 core\n"

		"in vec4 geomColor;\n"
		"in vec2 geomTexPosition;\n"

		"out vec4 fColor;\n"

		"void main() {\n"
		"	fColor = geomColor;\n"
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

		"layout(binding = 1)uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = texture(text, gl_FragCoord.xy);\n"
		"}");
	}

	std::string getTextFragment() {
		return std::string("#version 430 core\n"

		"in vec4 geomColor;\n"
		"in vec2 geomTexPosition;\n"

		"out vec4 fColor;\n"

		"uniform sampler2DRect text;\n"

		"void main() {\n"
		"	fColor = geomColor;\n"
		"	fColor.a *= texture(text, geomTexPosition).r;\n"
		"}");
	}
}
