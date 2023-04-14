#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoordinate;
layout(location = 3) in ivec4 boneIDs;
layout(location = 4) in vec4 weights;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoordinate;

out vec4 weights0;
flat out ivec4 boneIDs0;

uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 bones[200];
uniform bool isAnimated;

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

	gl_Position = viewMatrix * vertexPos;

	fragPos = vec3(vertexPos);
	fragNormal = normal;
	fragTexCoordinate = texCoordinate;
	boneIDs0 = boneIDs;
	weights0 = weights;
}