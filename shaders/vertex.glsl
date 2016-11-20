#version 430 core

layout(location=0)	in vec3 inPosition;
layout(location=1)	in vec2 inBounds;
layout(location=2)	in float inAngle;
layout(location=3)	in vec4 inColor;
layout(location=4)	in vec4 inTexData;

out QuadData {
	vec2 vBounds;
	float vAngle;
	vec4 vColor;
	vec4 vTexData;
};

out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
	vBounds = inBounds;
	vAngle = inAngle;
	vColor = inColor;
	vTexData = inTexData;
	gl_Position = vec4(inPosition, 1);
}
