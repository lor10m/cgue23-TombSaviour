#version 450 core

in float Height;
in vec4 textPos;

in vec3 fragPos;
in vec4 fragPosLightSpace;
//in vec4 fragNormal;

uniform sampler2D surfaceTexture;
uniform sampler2D shadowMap;

out vec4 FragColor;

float ShadowCalculation(vec4 posLightSpace)
{
    // perform perspective divide
    vec3 projCoords = posLightSpace.xyz / posLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{
    vec2 texCoord = textPos.xz;// / (1024 / 20);
    float h = (Height) / 205.0f;
    vec3 terrainColor = texture(surfaceTexture, texCoord).rgb * h;
    
    float shadow = ShadowCalculation(fragPosLightSpace);
    vec3 lighting = (1.0 - shadow) * terrainColor;
    
    FragColor = vec4(lighting, 1.0);
}
