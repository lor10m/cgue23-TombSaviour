#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordinate;

layout(location = 3) in vec3 tangent; //TODO location
layout(location = 4) in vec3 bitangent;

layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoordinate;
out vec4 fragPosLightSpace; // added for shadows;
out mat3 TBN; //for normal mapping

out vec4 weights0;
flat out ivec4 boneIDs0;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightSpaceMatrix; // added for shadows;
uniform mat4 bones[200];
uniform bool isAnimated;
uniform bool normalMapping;

void main()
{
	vec4 vertexPos = modelMatrix * vec4(position, 1.0);

	if (isAnimated) {
		mat4 boneTransform = bones[boneIDs[0]] * weights[0];
		boneTransform += bones[boneIDs[1]] * weights[1];
		boneTransform += bones[boneIDs[2]] * weights[2];
		boneTransform += bones[boneIDs[3]] * weights[3];
		vertexPos = (modelMatrix * boneTransform) * vec4(position, 1.0);
	}

	fragPos = vec3(vertexPos);
	//fragNormal = transpose(inverse(mat3(modelMatrix))) * normal;

	mat3 normalModelMatrix = mat3(transpose(inverse(modelMatrix)));
	fragNormal = normalize(normalModelMatrix * normal);

	if (normalMapping) {
		vec3 T = normalize(normalModelMatrix * tangent);
		vec3 B = normalize(normalModelMatrix * bitangent);
		TBN = mat3(T, B, fragNormal);
	}

	fragTexCoordinate = texCoordinate;
	fragPosLightSpace = lightSpaceMatrix * vertexPos;

	boneIDs0 = boneIDs;
	weights0 = weights;

	gl_Position = viewMatrix * vertexPos;
}