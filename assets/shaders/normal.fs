#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 interpTextCoords;
in mat3 TBN;
in vec4 fragPosLightSpace; // added for shadows

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

uniform vec3 eyePos; //cameraPos

uniform bool invertNormals = true;
uniform bool disableAttenuation = false;
uniform bool normalMapping = true;

struct Light
{
    vec3 position;
					
    vec3 ambientIntensity;
    vec3 diffuseIntensity;
    vec3 specularIntensity;

    float constant;
    float linear;
    float quadratic;
};	

uniform Light light;

out vec4 FragColor;


float calculateShadowFromDirectionalLight(vec3 normal);

void main() 
{
	vec3 normal;					

	if(normalMapping)
	{
		normal = texture(normalTexture, interpTextCoords).rgb;
		normal = normalize(2.0f * normal - 1.0f);
		normal = normalize(TBN * normal);
		normal = invertNormals ? -normal : normal;
	} 
	else 
	{
		normal = normalize(fragNormal);			
	}

	vec3 ambientLight = light.ambientIntensity;// * vec3(texture(diffuseTexture, interpTextCoords));
	vec3 toLight = normalize(light.position - fragPos);

	float shadow = calculateShadowFromDirectionalLight(normal);
	
	vec3 diffuseLight = max(dot(toLight, normal), 0) * light.diffuseIntensity * vec3(texture(diffuseTexture, interpTextCoords));

	vec3 toReflection = reflect(-toLight, normal);
	vec3 toView = normalize(eyePos - fragPos);
	float specularAmount = pow(max(dot(toView, toReflection), 0), 64); // 64 is shininess
	vec3 specularLight = light.specularIntensity * specularAmount * vec3(texture(specularTexture, interpTextCoords));

	float distance = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	attenuation = disableAttenuation ? 1 : attenuation;
	
	vec3 lightContribution = ambientLight + ((1.0 - shadow) * (diffuseLight + specularLight));// * vec3(attenuation);// * ((1.0 - shadow) + 0.3);

	FragColor = vec4(lightContribution, 1.0f);
}

float calculateShadowFromDirectionalLight(vec3 normal) 
{
    float shadow = 0.0f;
    // Convert fragment's light space position to normalized device coordinates (NDC)
    vec3 lightCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if(lightCoords.z <= 1.0f){
        // Transform NDC coordinates to texture coordinates
        lightCoords =  (lightCoords + 1.0f) / 2.0f;

        float currentDepth = lightCoords.z;

        vec3 lightDir = normalize(light.position - fragPos);//normalize(-directionalLight.direction);
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
