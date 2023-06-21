#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textCoords;

out vec2 HeightMapCoord;
out vec2 texCoord;

//added
out vec3 fragPos; 
//out vec4 fragPosLightSpace; // added for shadows;

uniform mat4 model;
uniform mat4 view;
//uniform mat4 lightSpaceMatrix; // added for shadows;

void main()
{
    vec4 vertexPos = model * vec4(position, 1.0);

    fragPos = vec3(vertexPos);

    HeightMapCoord = textCoords;
    gl_Position = vec4(position, 1.0);
}

//fragPosLightSpace = lightSpaceMatrix *  vec4(position,1.0);
//fragTexCoordinate = textCoords;
