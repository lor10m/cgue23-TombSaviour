#version 450 core

const int MAX_LIGHTS = 1;

struct PointLight {
    vec3 position;
    vec3 intensity;
    vec3 attenuation;
};

struct DirectionalLight {
    vec3 direction;
    vec3 intensity;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 intensity;
    vec3 attenuation;
    float angle;
};

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoordinate;
in vec4 fragPosLightSpace; // added for shadows
in mat3 TBN; // for normal mapping

flat in ivec4 boneIDs0;
in vec4 weights0;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
uniform sampler2D shadowMap; // added for shadows


uniform bool normalMapping = false;
uniform bool withShadow = true;

// ambient lighting factor
uniform float ka; //0.3

// diffuse lighting factor
uniform float kd; //0.3

// specular lighting factor
uniform float ks; //0.5

// specular factor
uniform int alpha; // 16

uniform vec3 lightPos; // added for shading
uniform vec3 eyePos;
uniform vec3 lightDir;

uniform PointLight pointLight[MAX_LIGHTS];
uniform int amountOfPointLights;
uniform DirectionalLight directionalLight[MAX_LIGHTS];
uniform int amountOfDirectionalLights;
uniform SpotLight spotLight[MAX_LIGHTS];
uniform int amountOfSpotLights;


float calculateShadowFromDirectionalLight(DirectionalLight directionalLight);
float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);

vec3 calculateDiffuseFromPointLight(PointLight pointLight);
vec3 calculateSpecularFromPointLight(PointLight pointLight);

vec3 calculateDiffuseFromSpotLight(SpotLight spotLight);
vec3 calculateSpecularFromSpotLight(SpotLight spotLight);

vec3 normal;

void main() 
{
    if(normalMapping)
    {
		normal = texture(normalTexture, fragTexCoordinate).rgb;
		normal = normalize(2.0f * normal - 1.0f);
		normal = normalize(TBN * normal);
		//normal = invertNormals ? -normal : normal;
	}
    else 
	{
		normal = normalize(fragNormal);			
	}

    float diffuse = 0.0;
    float specular = 0.0;
    float shadow = 0.0;

    // Directional lights
    for (int i = 0; i < amountOfDirectionalLights; i++) {
        diffuse += calculateDiffuseFromDirectionalLight(directionalLight[i]);
        specular += calculateSpecularFromDirectionalLight(directionalLight[i]);
        if(withShadow){
            shadow += calculateShadowFromDirectionalLight(directionalLight[i]);
        }
    }

    float ambient = ka;
    vec3 ambientLight = ambient * vec3(texture(diffuseTexture, fragTexCoordinate));
    vec3 diffuseLight = diffuse * vec3(texture(diffuseTexture, fragTexCoordinate));
    vec3 specularLight = specular * vec3(texture(specularTexture, fragTexCoordinate));
    // das da noch mal lightColor wenn man will; vlt intensity?
    //FragColor = texture(diffuseTexture, fragTexCoordinate) * vec4(diffuse,1.0f) + texture(specularTexture, fragTexCoordinate) * vec4(specular,1.0f);
    //FragColor = texture(diffuseTexture, fragTexCoordinate) * (diffuse + ka) + texture(specularTexture, fragTexCoordinate) * specular;

    vec3 lightContribution = ambientLight + (1.0 - shadow) * (diffuseLight + specularLight);
    FragColor = vec4(lightContribution, 1.0);
    
    // Point lights
    //for (int i = 0; i < amountOfPointLights; i++) {
    //    float shadow = calculateShadowFromDirectionalLight(directionalLight[i]);
    //    ambientIntensity += calculateDiffuseFromPointLight(pointLight[i]) * shadow;
    //    specularIntensity += calculateSpecularFromPointLight(pointLight[i]) * shadow;
    //}
    // Spot lights
    //for (int i = 0; i < amountOfSpotLights; i++) {
    //    ambientIntensity += calculateDiffuseFromSpotLight(spotLight[i]);
    //    specularIntensity += calculateSpecularFromSpotLight(spotLight[i]);
    //}
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

        //vec3 lightDir =  normalize(lightPos - fragPos);
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

float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    //vec3 L = normalize(-directionalLight.direction);
    vec3 L = lightDir;
    return max(dot(L, normal), 0) * kd;// * directionalLight.intensity * kd;
}

float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) {
    
    vec3 L = lightDir;
    vec3 R = normalize(reflect(-L, normal));
    vec3 V = normalize(eyePos - fragPos);
    //vec3 L = normalize(-directionalLight.direction);
    //vec3 R = 2 * dot(L, normal) * normal - L;
    //vec3 halwayDir = normalize(L + V);
    float specAmount = pow(max(dot(V, R), 0), alpha); // 64 is shininess
    //float specAmount = pow(max(dot(V, R), 0), alpha);
    //float specAmount = pow(max(dot(normal, halwayDir), 0), alpha);

    return specAmount * ks; //* directionalLight.intensity
}

vec3 calculateDiffuseFromPointLight(PointLight pointLight) {
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    return pointLight.intensity / attenuationFactor * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromPointLight(PointLight pointLight) {
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    return pointLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);
}

vec3 calculateDiffuseFromSpotLight(SpotLight spotLight) {
    // Directional vector to light source
    vec3 L = normalize(spotLight.position - vec3(fragPos));
    float attenuationFactor = spotLight.attenuation.x + spotLight.attenuation.y * distance(spotLight.position, vec3(fragPos)) + spotLight.attenuation.z * pow(distance(spotLight.position, vec3(fragPos)), 2);

    vec3 spotDir = normalize(-spotLight.direction);
    float intensity = smoothstep(0.0f, 0.02f, max(dot(L, spotDir) - cos(spotLight.angle), 0.0f));

    return intensity * spotLight.intensity / attenuationFactor * kd * max(dot(normal, L), 0.0f);
}

vec3 calculateSpecularFromSpotLight(SpotLight spotLight) {
    vec3 L = normalize(spotLight.position - vec3(fragPos));
    // Reflection
    vec3 R = 2 * dot(L, normal) * normal - L;
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    float attenuationFactor = spotLight.attenuation.x + spotLight.attenuation.y * distance(spotLight.position, vec3(fragPos)) + spotLight.attenuation.z * pow(distance(spotLight.position, vec3(fragPos)), 2);

    vec3 spotDir = normalize(-spotLight.direction);
    float intensity = smoothstep(0.0f, 0.02f, max(dot(L, spotDir) - cos(spotLight.angle), 0.0f));

    return intensity * spotLight.intensity / attenuationFactor * ks * pow(max(0, dot(R, V)), alpha);
}