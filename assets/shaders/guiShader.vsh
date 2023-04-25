#version 430 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordinate; //oder uvs 

//out VertexData{
//	vec3 positionWorld;
//	vec3 normalWorld;
//	vec2 uvs;
//} vert;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoordinate;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//uniform mat3 normalMat;

void main() {

	vec4 vertexPos = modelMatrix * vec4(position, 1.0);
	gl_Position = vertexPos;

	fragPos = vec3(vertexPos);
	fragNormal = normal;
	fragTexCoordinate = texCoordinate;

	//vert.normalWorld = normalMat * normal;
	//vert.uvs = vec2(uvs.x, 1.0 - uvs.y);
	//vec4 pos = modelMatrix * vec4(position, 1);
	//vert.positionWorld = pos.xyz;
	//gl_Position = viewProjectionMat * pos;
}
