#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordinate;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix; // added for shadows;

out vec3 fragNormal;
out vec3 fragPos;
out vec2 interpTextCoords;
out mat3 TBN;
out vec4 fragPosLightSpace; // added for shadows;

void main()
{

	fragPos = vec3(modelMatrix * vec4(position, 1.0));
	
	mat3 normalModelMatrix = mat3(transpose(inverse(modelMatrix)));
	fragNormal = normalize(normalModelMatrix * normal);
	vec3 T = normalize(normalModelMatrix * tangent);
	vec3 B = normalize(normalModelMatrix * bitangent);
	TBN = mat3(T, B, fragNormal);

	interpTextCoords = texCoordinate;

	vec4 vertexPos = modelMatrix * vec4(position, 1.0);

	fragPosLightSpace = lightSpaceMatrix * vertexPos; // added for shadow mapping
	gl_Position = viewMatrix * vertexPos;
}