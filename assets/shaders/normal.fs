#version 450 core

in vec3 fragNormal;
in vec3 fragPos;
in vec2 interpTextCoords;
in mat3 TBN;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;

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


void main() 
{
	vec3 ambientLight = light.ambientIntensity * vec3(texture(diffuseTexture, interpTextCoords));
	vec3 toLight = normalize(light.position - fragPos);

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

	vec3 diffuseLight = max(dot(toLight, normal), 0) * light.diffuseIntensity * vec3(texture(diffuseTexture, interpTextCoords));

	vec3 toReflection = reflect(-toLight, normal);
	vec3 toView = normalize(eyePos - fragPos);
	float specularAmount = pow(max(dot(toView, toReflection), 0), 64); // 64 is shininess
	vec3 specularLight = light.specularIntensity* specularAmount * vec3(texture(specularTexture, interpTextCoords));

	float distance = length(light.position - fragPos);
	float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	attenuation = disableAttenuation ? 1 : attenuation;
	
	vec3 lightContribution = (ambientLight + diffuseLight + specularLight) * vec3(attenuation);

	FragColor = vec4(lightContribution, 1.0f);
}