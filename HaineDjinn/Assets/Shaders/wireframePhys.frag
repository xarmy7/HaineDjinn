#version 450 core
// Varyings
in vec4 vColor;

// Shader outputs
out vec4 oColor;

void main()
{
    oColor = vColor;
}