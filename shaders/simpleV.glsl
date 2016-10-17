#version 430 core

layout(location=0)	in vec2 inPoint;
layout(location=1)	in vec3 inPosition;
layout(location=2)	in vec2 inBounds;
layout(location=3)	in vec4 inColor;
layout(location=4)	in vec4 inTexData;

out vec4 vColor;
out vec2 vTexPosition;

void main() {
	vColor = inColor;
	vTexPosition = inPoint * inTexData.zw + inTexData.xy;

	gl_Position = vec4(vec3(inPoint * inBounds, 0) + inPosition, 1);
}
