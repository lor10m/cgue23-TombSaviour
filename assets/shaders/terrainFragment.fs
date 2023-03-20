#version 450 core

in float Height;

uniform sampler2D surfaceTexture;
//ambient lighting factor
//uniform float ka;

out vec4 FragColor;

void main()
{
    // Ambient light
    vec3 lightIntensity = vec3(1, 1, 1) * 0.5f;
    vec3 specularIntensity = {0, 0, 0};

    float h = (Height + 16)/64.0f;
    FragColor = vec4(h, h, 0, 1.0) * vec4(lightIntensity, 1.0f);
    //FragColor = texture(surfaceTexture, texCoord);
    //FragColor = texture(surfaceTexture, texCoord);
    //FragColor = vec4(255.0, 255.0, 0.0, 1.0) * vec4(lightIntensity, 1.0f);
}