#version 450 core

uniform vec4 inColor;
out vec4 color;

void main() {
    color = vec4(inColor);
}