#version 430 core

in VertexData {
	vec3 positionWorld;
	vec3 normalWorld;
	vec2 uvs;
} vert;

layout(location = 0) out vec4 color;

uniform vec3 changeColor;
uniform sampler2D imgTexture;
uniform float brightness;

void main() {
	vec4 tex = texture(imgTexture, vert.uvs).rgba;
	color = tex;
	color.rbg += brightness;
}
