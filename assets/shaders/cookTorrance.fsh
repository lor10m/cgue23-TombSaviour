#version 450 core

const int MAX_LIGHTS = 16;
const float PI = 3.1415926535897932384626433832795;

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

out vec4 color;

uniform vec4 inColor;
// ambient lighting factor
uniform float ka;
// diffuse lighting factor
uniform float kd;
// ior
uniform float n;
// roughness
uniform float m;
uniform vec3 eyePos;
uniform PointLight pointLight[MAX_LIGHTS];
uniform int amountOfPointLights;
uniform DirectionalLight directionalLight[MAX_LIGHTS];
uniform int amountOfDirectionalLights;
uniform SpotLight spotLight[MAX_LIGHTS];
uniform int amountOfSpotLights;

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight);
vec3 calculateDiffuseFromPointLight(PointLight pointLight);
vec3 calculateSpecularFromPointLight(PointLight pointLight);
vec3 calculateDiffuseFromSpotLight(SpotLight spotLight);
vec3 calculateSpecularFromSpotLight(SpotLight spotLight);

vec3 normal;

void main() {
    normal = normalize(fragNormal);
    // Ambient light
    vec3 lightIntensity = vec3(1, 1, 1) * ka;
    vec3 specularIntensity = {0, 0, 0};

    // Point lights
    for (int i = 0; i < amountOfPointLights; i++) {
        lightIntensity += calculateDiffuseFromPointLight(pointLight[i]);
        specularIntensity += calculateSpecularFromPointLight(pointLight[i]);
    }

    // Directional lights
    for (int i = 0; i < amountOfDirectionalLights; i++) {
        lightIntensity += calculateDiffuseFromDirectionalLight(directionalLight[i]);
        specularIntensity += calculateSpecularFromDirectionalLight(directionalLight[i]);
    }

    // Spot lights
    for (int i = 0; i < amountOfSpotLights; i++) {
        lightIntensity += calculateDiffuseFromSpotLight(spotLight[i]);
        specularIntensity += calculateSpecularFromSpotLight(spotLight[i]);
    }

    color = inColor * vec4(lightIntensity, 1.0f) + vec4(specularIntensity, 1.0f);
}

vec3 calculateDiffuseFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    return directionalLight.intensity * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromDirectionalLight(DirectionalLight directionalLight) {
    // Directional vector to light source
    vec3 L = normalize(-directionalLight.direction);
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    // Halfway vector
    vec3 H = normalize(V + L);

    float NV = max(0, dot(V, normal));
    float VH = max(0, dot(V, H));
    float NL = max(0, dot(L, normal));
    float NH = max(0, dot(H, normal));
    float ks = 0;
    if (NL > 0) {
        // Geometric attenuation
        float G = min(1, min((2 * NH * NV) / VH, (2 * NH * NL) / VH));
        // Beckman distribution
        float D = exp((NH * NH - 1) / (NH * NH * m * m)) / (PI * m * m * pow(NH, 4));
        // Fresnel (Schlick's approximation)
        float r0 = pow((1 - n)/(1 + n), 2);
        float F = r0 + (1 - r0) * pow((1 - NV), 5);

        ks = (D * F * G) / (4 * NV * NL);
    }

    return ks * directionalLight.intensity;
}

vec3 calculateDiffuseFromPointLight(PointLight pointLight) {
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);
    return pointLight.intensity / attenuationFactor * kd * max(0, dot(L, normal));
}

vec3 calculateSpecularFromPointLight(PointLight pointLight) {
    // Directional vector to light source
    vec3 L = normalize(pointLight.position - vec3(fragPos));
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    // Halfway vector
    vec3 H = normalize(V + L);

    float attenuationFactor = pointLight.attenuation.x + pointLight.attenuation.y * distance(pointLight.position, vec3(fragPos)) + pointLight.attenuation.z * pow(distance(pointLight.position, vec3(fragPos)), 2);

    float NV = max(0, dot(V, normal));
    float VH = max(0, dot(V, H));
    float NL = max(0, dot(L, normal));
    float NH = max(0, dot(H, normal));
    float ks = 0;
    if (NL > 0) {
        // Geometric attenuation
        float G = min(1, min((2 * NH * NV) / VH, (2 * NH * NL) / VH));
        // Beckman distribution
        float D = exp((NH * NH - 1) / (NH * NH * m * m)) / (PI * m * m * pow(NH, 4));
        // Fresnel (Schlick's approximation)
        float r0 = pow((1 - n)/(1 + n), 2);
        float F = r0 + (1 - r0) * pow((1 - NV), 5);

        ks = (D * F * G) / (4 * NV * NL);
    }

    return ks * pointLight.intensity / attenuationFactor;
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
    // Directional vector to light source
    vec3 L = normalize(spotLight.position - vec3(fragPos));
    // Directional vector to viewer
    vec3 V = normalize(eyePos - vec3(fragPos));
    // Halfway vector
    vec3 H = normalize(V + L);
    float attenuationFactor = spotLight.attenuation.x + spotLight.attenuation.y * distance(spotLight.position, vec3(fragPos)) + spotLight.attenuation.z * pow(distance(spotLight.position, vec3(fragPos)), 2);

    vec3 spotDir = normalize(-spotLight.direction);
    float intensity = smoothstep(0.0f, 0.02f, max(dot(L, spotDir) - cos(spotLight.angle), 0.0f));

    float NV = max(0, dot(V, normal));
    float VH = max(0, dot(V, H));
    float NL = max(0, dot(L, normal));
    float NH = max(0, dot(H, normal));
    float ks = 0;
    if (NL > 0) {
        // Geometric attenuation
        float G = min(1, min((2 * NH * NV) / VH, (2 * NH * NL) / VH));
        // Beckman distribution
        float D = exp((NH * NH - 1) / (NH * NH * m * m)) / (PI * m * m * pow(NH, 4));
        // Fresnel (Schlick's approximation)
        float r0 = pow((1 - n)/(1 + n), 2);
        float F = r0 + (1 - r0) * pow((1 - NV), 5);

        ks = (D * F * G) / (4 * NV * NL);
    }

    return ks * intensity * spotLight.intensity / attenuationFactor;
}