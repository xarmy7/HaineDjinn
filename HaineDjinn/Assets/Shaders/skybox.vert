#version 450 core
const int LIGHTS_NUMBER = 8; // Note: this value must be the same as in ShaderData (RendererBasic.hpp)

// Attributes
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

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

// Uniforms
uniform mat4 uViewProj;

// Varyings (variables that are passed to fragment shader with perspective interpolation)
out vec3 vUV;

void main()
{
    vUV = vec3(aPosition.xy, -aPosition.z);
    vec4 pos = projectionMat * viewMat * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
}