#version 430 core

in vec4 geomColor;
in vec2 geomTexPosition;

out vec4 fColor;

void main() {

	fColor = geomColor;
}
