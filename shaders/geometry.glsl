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

const vec2 quad[4] = vec2[](vec2(-0.5, 0.5), vec2(-0.5, -0.5), vec2(0.5, 0.5), vec2(0.5, -0.5));

void main() {
	mat3 mat = mat3(	cos(inData[0].vAngle),	sin(inData[0].vAngle),	0,
							-sin(inData[0].vAngle),	cos(inData[0].vAngle),	0,
							0,								0,								1);

	for (int i = 0; i < 4; i++) {
		geomColor = inData[0].vColor;
		geomTexPosition = quad[i] * inData[0].vTexData.zw + inData[0].vTexData.xy;//hmmmm

		gl_Position = vec4(mat*vec3(quad[i] * inData[0].vBounds, 0) + gl_in[0].gl_Position.xyz, 1);
		EmitVertex();
	}

	EndPrimitive();
}
