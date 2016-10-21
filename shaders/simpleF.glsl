#version 430 core

in vec4 vColor;
in vec2 vTexPosition;

out vec4 fColor;

uniform sampler2DRect text;

void main() {

	fColor = texture(text, vTexPosition) * vColor;
}
