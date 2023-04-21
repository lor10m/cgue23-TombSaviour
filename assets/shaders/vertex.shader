#version 450 core

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
in vec3 position;

void main() {
    vec4 vertexPos = modelMatrix * vec4(position, 1.0);
    gl_Position = viewMatrix * vertexPos;
}