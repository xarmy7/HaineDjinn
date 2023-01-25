#version 450 core
const int LIGHTS_NUMBER = 8; // Note: this value must be the same as in ShaderData (RendererBasic.hpp)
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvCoords;

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

uniform mat4 uModelMat;
uniform mat4 viewProj;

out vec4 worldPos;
out vec4 normalDir;
out vec2 texCoords;

void main()
{
   mat4 viewProj = projectionMat * viewMat;
   worldPos = uModelMat * vec4(pos, 1.0);
   normalDir = normalize(uModelMat * vec4(normals, 0.0));
   texCoords = uvCoords;
   gl_Position = viewProj * uModelMat * vec4(pos.x, pos.y, pos.z, 1.0);
}