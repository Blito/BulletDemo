#version 330 core

layout(std140) uniform GlobalMatrices
{
    mat4 proj;
    mat4 view;
};

uniform mat4 pvm;
uniform mat4 model;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	Normal = normalize(vec3(view * model * vec4(normal,0.0)));	
	TexCoord = texCoord;
	gl_Position = pvm * vec4(position,1.0);
}