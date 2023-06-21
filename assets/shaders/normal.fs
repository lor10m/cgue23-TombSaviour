#version 450 core

const int MAX_LIGHTS = 1;

struct DirectionalLight {
    vec3 direction;
    vec3 intensity;
};


in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoordinate;
in vec4 fragPosLightSpace; // added for shadows
in mat3 TBN; // for normal mapping


out vec4 FragColor;


uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;
//uniform sampler2D shadowMap;

uniform bool invertNormals = false;
uniform bool normalMapping = true;


// ambient lighting factor
uniform float ka; //0.3

// diffuse lighting factor
uniform float kd; //0.3

// specular lighting factor
uniform float ks; //0.5

// specular factor
uniform int alpha; // 16

uniform vec3 lightPos; // added for shading
uniform vec3 eyePos; //cameraPos


uniform DirectionalLight directionalLight[MAX_LIGHTS];
uniform int amountOfDirectionalLights;

float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);

vec3 normal;

void main() 
{
						
	if(normalMapping)
	{
		normal = texture(normalTexture, fragTexCoordinate).rgb;
		normal = normalize(2.0f * normal - 1.0f);
		normal = normalize(TBN * normal);
		normal = invertNormals ? -normal : normal;
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
    }

    float ambient = ka;
    vec3 ambientLight = ambient * vec3(texture(diffuseTexture, fragTexCoordinate));
    vec3 diffuseLight = diffuse * vec3(texture(diffuseTexture, fragTexCoordinate));
    vec3 specularLight = specular * vec3(texture(specularTexture, fragTexCoordinate));

	//vec3 ambientLight = light.ambientIntensity;// * vec3(texture(diffuseTexture, interpTextCoords));
	//vec3 toLight = normalize(light.position - fragPos);

	//float shadow = calculateShadowFromDirectionalLight(normal);
	
	//vec3 diffuseLight = max(dot(toLight, normal), 0) * light.diffuseIntensity * vec3(texture(diffuseTexture, interpTextCoords));

	//vec3 toReflection = reflect(-toLight, normal);
	//vec3 toView = normalize(eyePos - fragPos);
	//float specularAmount = pow(max(dot(toView, toReflection), 0), 64); // 64 is shininess
	//vec3 specularLight = light.specularIntensity * specularAmount * vec3(texture(specularTexture, interpTextCoords));
	
    vec3 lightContribution = ambientLight + (1.0 - shadow) * (diffuseLight + specularLight);
    FragColor = vec4(lightContribution, 1.0);
}

float calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    //vec3 L = normalize(-directionalLight.direction);
    vec3 L = normalize(lightPos - fragPos);
    return max(dot(L, normal), 0) * kd;// * directionalLight.intensity * kd;
}

float calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) {
    
    vec3 L = normalize(lightPos - fragPos);
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
