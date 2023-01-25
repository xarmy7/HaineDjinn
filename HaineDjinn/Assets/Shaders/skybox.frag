#version 450 core

// Varyings
in vec3 vUV;

// Uniforms
uniform samplerCube skybox;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = texture(skybox, vUV);
}