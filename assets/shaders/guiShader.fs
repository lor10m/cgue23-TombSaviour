#version 430 core

//in VertexData {
//	vec3 positionWorld;
//	vec3 normalWorld;
//	vec2 uvs;
//} vert;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoordinate;

//layout(location = 0) out vec4 color;

out vec4 color;

uniform sampler2D diffuseTexture;

//uniform vec3 changeColor;
//uniform sampler2D imgTexture;
//uniform float brightness;

void main() {

	color = texture(diffuseTexture, fragTexCoordinate);

	//vec4 tex = texture(imgTexture, vert.uvs).rgba;
	//color = tex;
	//color.rbg += brightness;
}
