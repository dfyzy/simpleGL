#include "shaderData.hpp"

namespace simpleShaderData {

	std::string getVertex() {
		return std::string("#version 430 core\n"
		"layout(location=1)	in vec2 inPosition;\n"
		"layout(location=2)	in vec2 inBounds;\n"
		"layout(location=3)	in float inAngle;\n"
		"layout(location=4)	in vec4 inColor;\n"
		"layout(location=5)	in vec2 inTexPosition;\n"
		"layout(location=6)	in vec2 inTexBounds;\n"

		"out QuadData {\n"
		"	vec2 vBounds;\n"
		"	float vAngle;\n"
		"	vec4 vColor;\n"
		"	vec2 vTexPosition;\n"
		"	vec2 vTexBounds;\n"
		"};\n"

		"out gl_PerVertex {\n"
		"  vec4 gl_Position;\n"
		"};\n"

		"void main() {\n"
		"	vBounds = inBounds;\n"
		"	vAngle = inAngle;\n"
		"	vColor = inColor;\n"
		"	vTexPosition = inTexPosition + 0.5f*inTexBounds;\n"
		"	vTexBounds = inTexBounds;\n"
		"	gl_Position = vec4(inPosition, 0, 1);\n"
		"}");
	}

	std::string getDefaultGeometry() {
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
		"	vec2 inTexPosition;\n"
		"	vec2 inTexBounds;\n"
		"} inData[];\n"

		"out gl_PerVertex {\n"
		"	vec4 gl_Position;\n"
		"};\n"

		"out vec4 geomColor;\n"
		"out vec2 geomTexPosition;\n"

		"layout(std140) uniform DynamicData {\n"
		"	uniform vec2 cameraPosition;\n"
		"	uniform float cameraRotation;\n"
		"	uniform float cameraScale;\n"
		"	uniform vec2 resolution;\n"
		"} dynData;\n"

		"mat3 rotV, rotC;\n"

		"void drawVert(vec2 vert) {\n"
		"	geomColor = inData[0].vColor;\n"
		"	vec2 vertex = vert*inData[0].inTexBounds;\n"
		"	geomTexPosition = vertex + inData[0].inTexPosition;\n"

		"	gl_Position = vec4(rotC*(rotV*vec3(vertex * inData[0].vBounds * 2/dynData.resolution.y, 0)"
		"								+ vec3((gl_in[0].gl_Position.xy - dynData.cameraPosition) * 2/dynData.resolution.y, 0)), 1);\n"
		"	gl_Position.x *= dynData.resolution.y/dynData.resolution.x;\n"
		"	EmitVertex();\n"
		"}\n"

		"void main() {\n"
		"	float cosV = cos(inData[0].vAngle);\n"
		"	float sinV = sin(inData[0].vAngle);\n"
		"	rotV = mat3(cosV,		sinV,	0,\n"
		"					-sinV,	cosV,	0,\n"
		"					0,			0,		1);\n"

		"	float cosC = cos(-dynData.cameraRotation);\n"
		"	float sinC = sin(-dynData.cameraRotation);\n"
		"	rotC = dynData.cameraScale * mat3(	cosC,		sinC,	0,\n"
		"													-sinC,	cosC,	0,\n"
		"													0,			0,		1);\n"

		"	drawVert(vec2(-0.5, 0.5));\n"
		"	drawVert(vec2(-0.5, -0.5));\n"
		"	drawVert(vec2(0.5, 0.5));\n"
		"	drawVert(vec2(0.5, -0.5));\n"

		"	EndPrimitive();\n"
		"}");
	}

	std::string getLightingGeometry() {
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
		"	vec2 inTexPosition;\n"
		"	vec2 inTexBounds;\n"
		"} inData[];\n"

		"out gl_PerVertex {\n"
		"	vec4 gl_Position;\n"
		"};\n"

		"out vec4 geomColor;\n"
		"out vec2 geomTexPosition;\n"
		"out vec2 geomCentre;\n"
		"out vec2 geomBounds;\n"

		"layout(std140) uniform DynamicData {\n"
		"	uniform vec2 cameraPosition;\n"
		"	uniform float cameraRotation;\n"
		"	uniform float cameraScale;\n"
		"	uniform vec2 resolution;\n"
		"} dynData;\n"

		"mat3 rotV;\n"

		"void drawVert(vec2 vert) {\n"
		"	geomColor = inData[0].vColor;\n"
		"	geomCentre = gl_in[0].gl_Position.xy + dynData.resolution/2;\n"
		"	geomBounds = inData[0].inTexBounds;\n"
		"	vec2 vertex = vert*inData[0].inTexBounds;\n"
		"	geomTexPosition = vertex + inData[0].inTexPosition;\n"

		"	gl_Position = vec4(rotV*vec3(vertex * inData[0].vBounds * 2/dynData.resolution.y, 0)"
		"								+ vec3((gl_in[0].gl_Position.xy) * 2/dynData.resolution.y, 0), 1);\n"
		"	gl_Position.x *= dynData.resolution.y/dynData.resolution.x;\n"
		"	EmitVertex();\n"
		"}\n"

		"void main() {\n"
		"	float cosV = cos(inData[0].vAngle);\n"
		"	float sinV = sin(inData[0].vAngle);\n"
		"	rotV = mat3(cosV,		sinV,	0,\n"
		"					-sinV,	cosV,	0,\n"
		"					0,			0,		1);\n"

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

	std::string getLightingDefaultFragment() {
		return std::string("#version 430 core\n"

		"in vec4 geomColor;\n"
		"in vec2 geomTexPosition;\n"
		"in vec2 geomCentre;\n"
		"in vec2 geomBounds;\n"

		"out vec4 fColor;\n"

		"void main() {\n"
		"	fColor = geomColor;\n"
		"}");
	}

	std::string getLightingPow2Fragment() {
		return std::string("#version 430 core\n"

		"in vec4 geomColor;\n"
		"in vec2 geomTexPosition;\n"
		"in vec2 geomCentre;\n"
		"in vec2 geomBounds;\n"

		"out vec4 fColor;\n"

		"void main() {\n"
		"	fColor = (1 - pow(2*length((geomCentre - gl_FragCoord.xy)/geomBounds), 2))*geomColor;\n"
		"}");
	}

}
