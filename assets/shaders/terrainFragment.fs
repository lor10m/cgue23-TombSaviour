#version 450 core

const int MAX_LIGHTS = 1;

struct DirectionalLight {
    vec3 direction;
    vec3 intensity;
};

in float Height;
in vec4 textPos; //position from tes
in vec3 normal; //normals from tes

in vec3 fragPos; //
in vec2 fragTexCoords; //
in vec4 fragPosLightSpace; // added for shadows;

uniform sampler2D heightMap;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

out vec4 FragColor;

uniform float ka; //0.3
uniform float kd; //0.3
uniform float ks; //0.5
uniform int alpha; // 16

uniform vec3 lightPos; // added for shading
uniform vec3 eyePos; //

uniform DirectionalLight directionalLight[MAX_LIGHTS]; //
uniform int amountOfDirectionalLights; //

float calculateShadowFromDirectionalLight(DirectionalLight directionalLight);
float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);

//vec3 normal;

float getHeightValue(vec2 coords)
{
    float n = 0.33;
    return max(n, texture(heightMap, coords).x);
}


void main()
{
    vec2 texCoord = fragTexCoords;// / (1024 / 20);

//    float intensity = 0.2;
//    float offset = 1/1024;
//    vec3 a = vec3(texCoord.x - offset, 0.0, getHeightValue(vec2(texCoord.x - offset, texCoord.y)) * intensity);
//    vec3 b = vec3(texCoord.x + offset, 0.0, getHeightValue(vec2(texCoord.x + offset, texCoord.y)) * intensity);
//    vec3 c = vec3(0.0, texCoord.y + offset, getHeightValue(vec2(texCoord.x, texCoord.y + offset)) * intensity);
//    vec3 d = vec3(0.0, texCoord.y - offset, getHeightValue(vec2(texCoord.x, texCoord.y - offset)) * intensity);

    //normal = normalize(cross(b-a, c-d));

    float diffuse = 0.0;
    float specular = 0.0;
    float shadow = 0.0;

    // Directional lights
    for (int i = 0; i < 1; i++) {
        diffuse += calculateDiffuseFromDirectionalLight(directionalLight[i]);
        specular += calculateSpecularFromDirectionalLight(directionalLight[i]);
        shadow += calculateShadowFromDirectionalLight(directionalLight[i]);
    }

    float ambient = ka;
    vec3 ambientLight = ambient * vec3(texture(diffuseTexture, fragTexCoords));
    vec3 diffuseLight = diffuse * vec3(texture(diffuseTexture, fragTexCoords));
    vec3 specularLight = specular * vec3(texture(specularTexture, fragTexCoords));
    
    vec3 lightContribution = ambientLight + (1.0 - shadow) * (diffuseLight + specularLight);
    FragColor = vec4(lightContribution, 1.0);

    //float h = (Height) / 205.0f;
    //vec4 terrainColor = texture(surfaceTexture, texCoord) * h;
    //FragColor = vec4(terrainColor.rgb, 1.0);
    //FragColor = vec4(h, h, 0, 1.0) * vec4(lightIntensity, 1.0f);
}

float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) 
{
    vec3 L = normalize(lightPos - vec3(textPos)); //fragpos
    return max(dot(L, normal), 0) * kd;
}

float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) 
{
    vec3 L = normalize(lightPos - vec3(textPos)); // fragpos
    vec3 R = normalize(reflect(-L, normal));
    vec3 V = normalize(eyePos -  vec3(textPos)); //specular works fine with fragPos
    float specAmount = pow(max(dot(V, R), 0), alpha); // 64 is shininess

    return specAmount * ks;
}

float calculateShadowFromDirectionalLight(DirectionalLight directionalLight) 
{
    float shadow = 0.0f;
    // Convert fragment's light space position to normalized device coordinates (NDC)
    vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if(lightCoords.z <= 1.0f){
        // Transform NDC coordinates to texture coordinates
        lightCoords =  (lightCoords + 1.0f) / 2.0f;

        float currentDepth = lightCoords.z;

        vec3 lightDir =  normalize(lightPos - vec3(textPos));
        float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

        int sampleRadius = 1;
        vec2 pixelSize = 1.0/textureSize(shadowMap,0);
        
        for(int x = -sampleRadius; x <= sampleRadius; ++x)
		{
		    for(int y = -sampleRadius; y <= sampleRadius; ++y)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth - bias > closestDepth)
					shadow += 1.0f;
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }

	// Return shadow value
	return shadow;
}