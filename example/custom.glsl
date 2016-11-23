#version 430 core

in vec4 vColor;
in vec2 vTexPosition;

out vec4 fColor;

uniform sampler2DRect text;
uniform vec4 color = vec4(0);

void main() {

	fColor = texture(text, vTexPosition) * color;
}
