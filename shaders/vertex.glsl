//For each quad this shader will be called 4 times. For this 4 times the only thing that will change is inPoint.
#version 430 core

//this will be (-0.5, 0.5), (-0.5, -0.5), (0.5, 0.5), (0.5, -0.5) for each corner of the quad
layout(location=0)	in vec2 inPoint;
//position of a centre of the quad
layout(location=1)	in vec3 inPosition;
//width and height of the quad
layout(location=2)	in vec2 inBounds;
//rotation of the quad
layout(location=3)	in float inAngle;
//color of the quad
layout(location=4)	in vec4 inColor;
//vector with first two members x and y of a quad inside a texture that we will be sampling. the other two is width and height.
layout(location=5)	in vec4 inTexData;

out vec4 vColor;
out vec2 vTexPosition;

out gl_PerVertex {
  vec4 gl_Position;
};

void main() {
	vColor = inColor;
	vTexPosition = inPoint * inTexData.zw + inTexData.xy;//hmmmm

	mat3 mat = mat3(	cos(inAngle),	sin(inAngle),	0,
							-sin(inAngle),	cos(inAngle),	0,
							0,					0,					1);

	gl_Position = vec4(mat*vec3(inPoint * inBounds, 0) + inPosition, 1);
}
