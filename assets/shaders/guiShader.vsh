#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvs;

out VertexData{
	vec3 positionWorld;
	vec3 normalWorld;
	vec2 uvs;
} vert;

uniform mat4 modelObjMat;
uniform mat4 viewProjectionMat;
uniform mat3 normalMat;

void main() {
	vert.normalWorld = normalMat * normal;
	vert.uvs = vec2(uvs.x, 1.0 - uvs.y);
	vec4 pos = modelObjMat * vec4(position, 1);
	vert.positionWorld = pos.xyz;
	gl_Position = viewProjectionMat * pos;
}
