#version 430 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

in gl_PerVertex {
  vec4 gl_Position;
} gl_in[];

in QuadData {
	vec2 vBounds;
	float vAngle;
	vec4 vColor;
	vec4 vTexData;
} inData[];

out gl_PerVertex {
	vec4 gl_Position;
};

out vec4 geomColor;
out vec2 geomTexPosition;

layout(std140) uniform StaticData {
	uniform float rAspect;
} stData;

layout(std140) uniform DynamicData {
	uniform vec2 cameraPosition;
	uniform float cameraRotation;
} dynData;

const vec2 quad[4] = vec2[](vec2(-0.5, 0.5), vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(0.5, -0.5));

void main() {
	float cosV = cos(inData[0].vAngle);
	float sinV = sin(inData[0].vAngle);
	mat3 rotV = mat3(	cosV,		sinV,	0,
							-sinV,	cosV,	0,
							0,			0,		1);

	float cosC = cos(-dynData.cameraRotation);
	float sinC = sin(-dynData.cameraRotation);
	mat3 rotC = mat3(	cosC,		sinC,	0,
							-sinC,	cosC,	0,
							0,			0,		1);

	for (int i = 0; i < 4; i++) {
		geomColor = inData[0].vColor;
		geomTexPosition = quad[i] * inData[0].vTexData.zw + inData[0].vTexData.xy;//hmmmm

		gl_Position = vec4(rotC*(rotV*vec3(quad[i] * inData[0].vBounds, 0) + vec3(gl_in[0].gl_Position.xy - dynData.cameraPosition, 0)), 1);
		gl_Position.x *= stData.rAspect;
		EmitVertex();
	}

	EndPrimitive();
}
