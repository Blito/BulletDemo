#version 330 core

layout(std140) uniform GlobalMatrices
{
    mat4 proj;
    mat4 view;
};

uniform mat4 pvm;
uniform mat4 model;

in vec3 position;
in vec3 color;

out vec3 Color;

void main() {
	Color = color;
	gl_Position = pvm * vec4(position, 1.0);
}