#version 430 core

in vec4 geomColor;
in vec2 geomTexPosition;

out vec4 fColor;

uniform sampler2DRect text;

void main() {

	fColor = texture(text, geomTexPosition) * geomColor;
}
