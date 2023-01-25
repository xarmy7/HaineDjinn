#version 450 core
const int LIGHTS_NUMBER = 8; // Note: this value must be the same as in ShaderData (RendererBasic.hpp)

struct Light
{
    vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
    vec4 attenuation;
    bool enabled;
};

layout(std140, binding = 0) uniform shaderData
{
    Light lights[LIGHTS_NUMBER];
    mat4 projectionMat;
    mat4 viewMat;
    vec4 camPos;
};

uniform sampler2D uColorTexture;
in vec4 worldPos;
in vec4 normalDir;
in vec2 texCoords;
out vec4 FragColor;

float getDiffuseFactor(vec3 lightDir, vec3 normal)
{
    return max(dot(lightDir, normal), 0.0);
}

float getSpecularFactor(vec3 lightDir, vec3 normal, vec3 camPos, vec3 worldPos)
{
    vec3 reflection = reflect(-lightDir, normal);
    return max(dot(reflection, normalize(camPos - worldPos)), 0.0);
}

// MAIN
void main()
{
    vec3 ambiantColor = vec3(0, 0, 0);
    vec3 diffuseColor = vec3(0, 0, 0);
    vec3 specularColor = vec3(0);

    vec3 ka = vec3(1.0, 1.0, 1.0); // TODO: Replace with material values
    vec3 kd = vec3(1.0, 1.0, 1.0); // TODO: Replace with material values
    vec3 ks = vec3(1.0, 1.0, 1.0); // TODO: Replace with material values
    float shininess = 128;

    vec3 fragNormal = cross(vec3(normalDir), vec3(worldPos));

    for (int i = 0; i < LIGHTS_NUMBER; i++)
        if (lights[i].enabled)
        {
            float dist = length(vec3(lights[i].position) - vec3(worldPos));
            float attenuation = 1 / (lights[i].attenuation.x + lights[i].attenuation.y * dist + lights[i].attenuation.z * pow(dist, 2.f));
            
            vec3 la = vec3(lights[i].ambient);// * attenuation;  // ambient color
            vec3 ld = vec3(lights[i].diffuse) * attenuation;  // diffuse color
            vec3 ls = vec3(lights[i].specular) * attenuation; // specular color
            
            vec3 l = normalize(vec3(lights[i].position) - vec3(worldPos));

            ambiantColor = ambiantColor + la * ka;
            diffuseColor = diffuseColor + kd * getDiffuseFactor(l, vec3(normalDir)) * ld;

            specularColor = specularColor + ks * pow(getSpecularFactor(l, vec3(normalDir), vec3(camPos), vec3(worldPos)), shininess) * ls;
        }

    vec3 shadedColor = diffuseColor + ambiantColor + specularColor;
    FragColor = vec4(shadedColor, 1.0) * texture(uColorTexture, texCoords);
}